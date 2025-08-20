use std::collections::{HashMap, HashSet};

use crate::bibtool::BibEntry;

/// 计算两个字符串的相似度（基于 Levenshtein 编辑距离）
pub fn string_similarity(str1: &str, str2: &str) -> f64 {
    let len1 = str1.len();
    let len2 = str2.len();

    if len1 == 0 && len2 == 0 {
        return 1.0;
    }
    if len1 == 0 || len2 == 0 {
        return 0.0;
    }

    let mut prev: Vec<usize> = (0..=len2).collect();
    let mut curr: Vec<usize> = vec![0; len2 + 1];

    for (i, c1) in str1.chars().enumerate() {
        curr[0] = i + 1;
        for (j, c2) in str2.chars().enumerate() {
            let cost = if c1.to_ascii_lowercase() == c2.to_ascii_lowercase() {
                0
            } else {
                1
            };

            curr[j + 1] = std::cmp::min(
                std::cmp::min(prev[j + 1] + 1, curr[j] + 1),
                prev[j] + cost,
            );
        }
        std::mem::swap(&mut prev, &mut curr);
    }

    1.0 - (prev[len2] as f64) / (std::cmp::max(len1, len2) as f64)
}

/// 查找标题重复的 BibEntry
pub fn find_duplicate_entries(
    bib_entries: &[BibEntry],
    threshold: f64,
) -> Vec<(String, String)> {
    let mut duplicates = Vec::new();

    for i in 0..bib_entries.len() {
        for j in 0..bib_entries.len() {
            if i == j {
                continue;
            }
            if let (Some(title1), Some(title2)) =
                (bib_entries[i].title(), bib_entries[j].title())
            {
                if string_similarity(title1, title2) > threshold {
                    duplicates.push((
                        bib_entries[i].name().to_string(),
                        bib_entries[j].name().to_string(),
                    ));
                }
            }
        }
    }

    duplicates
}

/// 查找缩写与 venue 名称的重复
pub fn find_duplicate_abbr(
    bib_abbr: &HashMap<String, String>,
    bib_entries: &[BibEntry],
    threshold: f64,
) -> Vec<(String, String)> {
    let mut duplicates = Vec::new();

    for entry in bib_entries {
        if let Some(venue) = entry.venue_name() {
            for (key, value) in bib_abbr {
                if string_similarity(venue, value) > threshold {
                    duplicates.push((entry.name().to_string(), key.clone()));
                }
            }
        }
    }

    duplicates
}

/// 检查同 venue 的 BibEntry 是否有字段不一致
pub fn check_field(
    bib_entries: &[BibEntry],
    threshold: f64,
) -> Vec<(String, String)> {
    let mut res = Vec::new();
    let mut done = vec![false; bib_entries.len()];

    for i in 0..bib_entries.len() {
        if done[i] {
            continue;
        }

        let field_key: HashSet<&String> = bib_entries[i].field_keys();
        done[i] = true;

        for j in 0..bib_entries.len() {
            if i == j {
                continue;
            }

            if let (Some(venue1), Some(venue2)) =
                (bib_entries[i].venue_name(), bib_entries[j].venue_name())
            {
                if string_similarity(venue1, venue2) > threshold {
                    if field_key != bib_entries[j].field_keys() {
                        res.push((
                            bib_entries[i].name().to_string(),
                            bib_entries[j].name().to_string(),
                        ));
                    }
                    done[j] = true;
                }
            }
        }
    }

    res
}
