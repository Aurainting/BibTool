use std::collections::{HashMap, HashSet};

pub struct BibEntry {
    entry_type: String,
    name: String,
    fields: HashMap<String, String>,
}

impl BibEntry {
    pub fn new(entry_type: String) -> Self {
        Self {
            entry_type,
            name: String::new(),
            fields: HashMap::new(),
        }
    }

    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn name_mut(&mut self) -> &mut String {
        &mut self.name
    }

    pub fn add_field(&mut self, key: String, value: String) {
        self.fields.insert(key, value);
    }

    pub fn title(&self) -> Option<&str> {
        if let Some(val) = self.fields.get("title") {
            Some(val.as_str())
        } else {
            self.fields.get("TITLE").map(|s| s.as_str())
        }
    }

    pub fn venue_name(&self) -> Option<&str> {
        for key in [
            "journal", "JOURNAL",
            "booktitle", "BOOKTITLE",
            "howpublished", "HOWPUBLISHED",
        ] {
            if let Some(val) = self.fields.get(key) {
                return Some(val.as_str());
            }
        }
        None
    }

    pub fn field_keys(&self) -> HashSet<&String> {
        self.fields.keys().collect()
    }
}
