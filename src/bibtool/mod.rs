pub mod entry;
pub mod parse;
pub mod check;

pub use entry::BibEntry;
pub use parse::{type_mode, abbr_mode, entry_mode};
pub use check::{find_duplicate_entries, find_duplicate_abbr, check_field};
