//..................................................................................................
//
// $BeginLicense$
// $EndLicense$
//
//..................................................................................................

#pragma once
 
namespace cb_constants 
	{
    const QString application_name    ("cb_find_duplicates");
    const QString application_version ("0.1.0");
    const QString organization_name   ("cb_software");
    const QString domain_name         ("camiel.bouchier.be");

    namespace log
        {
        // In AppDataLocal
        const QString logdir_name  ("logs");
        const int     days_to_keep (5);
        const int     max_entries  (20);
        }
	}

//..................................................................................................

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
