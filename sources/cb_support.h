//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
//
// (C) 2015-2021 by Camiel Bouchier (camiel@bouchier.be)
//
// This file is part of cb_find_duplicates.
// All rights reserved.
// You are granted a non-exclusive and non-transferable license to use this
// software for personal or internal business purposes.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Camiel Bouchier BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
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
