//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#pragma once

const uint64_t kilo = 1024;
const uint64_t mega = kilo*kilo;
const uint64_t giga = kilo*mega;

bool cb_hardlink(const QString& existing_file, const QString& new_file);
bool cb_is_same_file(const QString& file1, const QString& file2);
bool cb_is_same_file(const QStringList& files);

const QString cb_md5_sum(const QString& file, const bool partial, bool& ok);
const QString cb_represent(const QStringList& string_list);
const QString cb_sizestring_from_size(const uint64_t size);
const QString cb_sizestring_from_key(const QString& key);

uint64_t cb_size_from_key(const QString& key);
uint64_t cb_get_fake_inode(const QString& file);

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
