use std::collections::HashMap;

use crate::bibtool::BibEntry;

pub fn type_mode<I>(it: &mut I) -> String
where
    I: Iterator<Item = char> + Clone,
{
    let mut type_name = String::new();
    while let Some(&c) = it.clone().next().as_ref() {
        if c.is_alphanumeric() {
            type_name.push(c);
            it.next();
        } else {
            break;
        }
    }
    type_name
}

pub fn abbr_mode<I>(it: &mut I, bib_abbr: &mut HashMap<String, String>)
where
    I: Iterator<Item = char> + Clone,
{
    // Skip until '{'
    while let Some(c) = it.next() {
        if c == '{' {
            break;
        }
    }

    let mut key = String::new();
    while let Some(&c) = it.clone().next().as_ref() {
        if c.is_alphanumeric() {
            key.push(c);
            it.next();
        } else {
            break;
        }
    }

    // Skip until value delimiter
    while let Some(c) = it.clone().next() {
        if c == '"' || c == '{' {
            break;
        }
        it.next();
    }
    it.next(); // consume the delimiter

    let mut value = String::new();
    while let Some(c) = it.clone().next() {
        if c == '"' || c == '}' {
            break;
        }
        value.push(c);
        it.next();
    }

    bib_abbr.insert(key, value);
}

pub fn entry_mode<I>(it: &mut I, entry: &mut BibEntry)
where
    I: Iterator<Item = char> + Clone,
{
    let mut stack = Vec::new();

    // Skip until '{'
    while let Some(c) = it.next() {
        if c == '{' {
            break;
        }
    }
    stack.push('{');

    // Read entry name until ','
    entry.name_mut().clear();
    while let Some(c) = it.clone().next() {
        if c == ',' {
            break;
        }
        entry.name_mut().push(c);
        it.next();
    }
    it.next(); // consume ','

    let mut key = String::new();
    let mut word = String::new();

    while !stack.is_empty() {
        let c = match it.next() {
            Some(ch) => ch,
            None => break,
        };

        if c == '\\' {
            word.push(c);
            if let Some(next) = it.next() {
                word.push(next);
            }
        } else if c.is_alphanumeric() {
            word.push(c);
        } else if c == '{' || (c == '"' && *stack.last().unwrap() != '"') {
            if stack.len() > 1 {
                word.push(c);
            }
            stack.push(c);
        } else if c == '}' || (c == '"' && *stack.last().unwrap() == '"') {
            if stack.len() > 2 {
                word.push(c);
            }
            if c == '}' && *stack.last().unwrap() != '{' {
                panic!("Parse error: mismatched braces");
            }
            stack.pop();
        } else if stack.len() == 1 && c == ',' {
            if !key.is_empty() {
                entry.add_field(key.clone(), word.clone());
                key.clear();
                word.clear();
            }
        } else if stack.len() == 1 && c == '=' {
            key = word.clone();
            word.clear();
        } else {
            if stack.len() > 1 {
                word.push(c);
            }
        }
    }

    if !key.is_empty() && !word.is_empty() {
        entry.add_field(key, word);
    }
}
