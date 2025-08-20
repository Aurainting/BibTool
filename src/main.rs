use std::fs;
use std::collections::HashMap;

mod bibtool;
use bibtool::{BibEntry, abbr_mode, entry_mode, type_mode, check_field, find_duplicate_abbr, find_duplicate_entries};

fn main() {
    // Read bib file
    let bib_str = fs::read_to_string("example/references.bib")
        .expect("Should have been able to read the file");

    let mut bib_abbr: HashMap<String, String> = HashMap::new();
    let mut bib_entries: Vec<BibEntry> = Vec::new();

    let mut it = bib_str.chars().peekable();
    while let Some(c) = it.next() {
        if c == '@' {
            let entry_type = type_mode(&mut it);

            if entry_type == "string" || entry_type == "STRING" {
                abbr_mode(&mut it, &mut bib_abbr);
            } else {
                let mut entry = BibEntry::new(entry_type);
                entry_mode(&mut it, &mut entry);
                bib_entries.push(entry);
            }
        }
    }

    // Do some checks
    let duplicates = find_duplicate_entries(&bib_entries, 0.85);
    if !duplicates.is_empty() {
        for (i, j) in duplicates {
            println!("{} and {} are duplicated!", i, j);
        }
        println!();
    }

    let duplicates = find_duplicate_abbr(&bib_abbr, &bib_entries, 0.85);
    if !duplicates.is_empty() {
        for (i, j) in duplicates {
            println!("{} and {} are duplicated!", i, j);
        }
        println!();
    }

    let mis_field = check_field(&bib_entries, 0.85);
    if !mis_field.is_empty() {
        for (i, j) in mis_field {
            println!("{} and {} field keys are not the same!", i, j);
        }
        println!();
    }
}
