//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include <QDebug>
#include <QFileInfo>
#include <QtGlobal>
#include <QStringList>

#include "cb_abort.h"
#include "cb_support.h"

#if defined(Q_OS_WIN)
    #include <windows.h>
#elif defined(Q_OS_LINUX)
    #include <sys/stat.h>
    #include <unistd.h>
#else
    #error Unsupported OS.
#endif

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

QString cb_md5_sum(const QString& filename, const bool partial, bool& ok)
    {
    if (not QFileInfo(filename).isReadable())
        {
        auto error_msg = QString("Could not read '%1'.").arg(filename);
        qWarning() << error_msg;
        ok = false;
        return QString();
        }
  
    QFile file(filename);
    if (not file.open(QIODevice::ReadOnly))
        {
        auto error_msg = QString("Could not open '%1'.").arg(filename);
        qWarning() << error_msg;
        ok = false;
        return QString();
        }

    auto file_size = file.size();
    auto target_chunk_size = partial ? 4 * 1024 * 1024 : 128 * 1024 * 1024;
    auto chunk_size = (file_size < target_chunk_size) ? file_size : target_chunk_size;
    QCryptographicHash hash(QCryptographicHash::Md5);
    while (not file.atEnd())
        {
        auto chunk = file.read(chunk_size);
        hash.addData(chunk);
        if (partial) break;
        }
    auto md5 = QString(hash.result().toHex());
    ok = true;
    return md5;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Info from Windows API :
// typedef struct _BY_HANDLE_FILE_INFORMATION {
//   DWORD    dwFileAttributes;
//   FILETIME ftCreationTime;
//   FILETIME ftLastAccessTime;
//   FILETIME ftLastWriteTime;
//   DWORD    dwVolumeSerialNumber;    (*)
//   DWORD    nFileSizeHigh;
//   DWORD    nFileSizeLow;
//   DWORD    nNumberOfLinks;
//   DWORD    nFileIndexHigh;          (*)
//   DWORD    nFileIndexLow;           (*)
// } BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION;
// 
// (*) together defines unique file.

//  Info from Linux :
//  struct stat {
//      dev_t     st_dev;     /* ID of device containing file */           (*)
//      ino_t     st_ino;     /* inode number */                           (*)
//      mode_t    st_mode;    /* protection */
//      nlink_t   st_nlink;   /* number of hard links */
//      uid_t     st_uid;     /* user ID of owner */
//      gid_t     st_gid;     /* group ID of owner */
//      dev_t     st_rdev;    /* device ID (if special file) */
//      off_t     st_size;    /* total size, in bytes */
//      blksize_t st_blksize; /* blocksize for file system I/O */
//      blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
//      time_t    st_atime;   /* time of last access */
//      time_t    st_mtime;   /* time of last modification */
//      time_t    st_ctime;   /* time of last status change */
//  };
//
// (*) together defines unique file.

#if defined(Q_OS_WIN)

    struct cb_file_identifier
        {
        DWORD    dwVolumeSerialNumber;
        DWORD    nFileIndexHigh;
        DWORD    nFileIndexLow;
        friend bool operator == (const cb_file_identifier& x1, const cb_file_identifier& x2);
        friend bool operator != (const cb_file_identifier& x1, const cb_file_identifier& x2);
        };

    bool operator == (const cb_file_identifier& x1, const cb_file_identifier& x2)
        {
        return (x1.dwVolumeSerialNumber == x2.dwVolumeSerialNumber) and
               (x1.nFileIndexHigh == x2.nFileIndexHigh) and
               (x1.nFileIndexLow == x2.nFileIndexLow);
        }

    bool operator != (const cb_file_identifier& x1, const cb_file_identifier& x2)
        {
        return !(x1 == x2);
        }

#elif defined(Q_OS_LINUX)

    struct cb_file_identifier
        {
        dev_t     st_dev;  
        ino_t     st_ino;  
        friend bool operator == (const cb_file_identifier& x1, const cb_file_identifier& x2);
        friend bool operator != (const cb_file_identifier& x1, const cb_file_identifier& x2);
        };

    bool operator == (const cb_file_identifier& x1, const cb_file_identifier& x2)
        {
        return (x1.st_dev == x2.st_dev) and
               (x1.st_ino == x2.st_ino);
        }
  
    bool operator != (const cb_file_identifier& x1, const cb_file_identifier& x2)
        {
        return !(x1 == x2);
        }

#else
  #error Unsupported OS.
#endif

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_file_identifier cb_get_file_identifier(const QString& file)
    {
    cb_file_identifier identifier;

    #if defined(Q_OS_WIN)

        BY_HANDLE_FILE_INFORMATION lpFileInformation;
        HANDLE fileHandle;
        fileHandle = CreateFileW(file.toStdWString().c_str(),
                                 FILE_READ_ATTRIBUTES, 
                                 FILE_SHARE_READ,
                                 NULL,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_BACKUP_SEMANTICS,
                                 NULL);
        int success = GetFileInformationByHandle(fileHandle, &lpFileInformation);
        CloseHandle(fileHandle);

        identifier.dwVolumeSerialNumber = lpFileInformation.dwVolumeSerialNumber;
        identifier.nFileIndexHigh       = lpFileInformation.nFileIndexHigh;
        identifier.nFileIndexLow        = lpFileInformation.nFileIndexLow; 

        if (not success)
            {
            identifier.dwVolumeSerialNumber = 0;
            identifier.nFileIndexHigh       = 0;
            identifier.nFileIndexLow        = 0;
            }

    #elif defined(Q_OS_LINUX)

        struct stat info;
        bool success = (0 == stat(qPrintable(file), &info));

        identifier.st_dev = info.st_dev;
        identifier.st_ino = info.st_ino;

    #else
        #error Unsupported OS.
    #endif
    
    if (not success)
        {
        auto err_msg = QString("Unforeseen non-success in '%1', file '%2', reason '%3'")
                       .arg(__PRETTY_FUNCTION__)
                       .arg(file).
                       arg(strerror(errno));
        qWarning() << err_msg;
        }
    
    return identifier;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bool cb_is_same_file(const QString& file1, const QString& file2)
    {
    auto is_same = (cb_get_file_identifier(file1) == cb_get_file_identifier(file2));
    return is_same;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bool cb_is_same_file(const QStringList& files)
    {
    auto nr_files = files.size();
    if (nr_files == 0) 
        {
        auto err_msg = QObject::tr("nr_files:0");
        ABORT(err_msg);
        }
    auto ref_identifier = cb_get_file_identifier(files.at(0));
    auto is_same = true;
    for (int idx=1; idx<nr_files; idx++)
        {
        if (cb_get_file_identifier(files.at(idx)) != ref_identifier)
            {
            is_same = false;
            break;
            }
        }
    return is_same;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

uint64_t cb_get_fake_inode(const QString& file)
    {
    uint64_t fake_inode;

    auto identifier = cb_get_file_identifier(file);

    #if defined(Q_OS_WIN)

        fake_inode =  (uint64_t) identifier.nFileIndexHigh << 32 | identifier.nFileIndexLow; 
        fake_inode ^= (uint64_t) identifier.dwVolumeSerialNumber;

    #elif defined(Q_OS_LINUX)

        fake_inode  = (uint64_t) identifier.st_dev;
        fake_inode ^= (uint64_t) identifier.st_ino;

    #else
        #error Unsupported OS.
    #endif
   
    return fake_inode; 
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bool cb_hardlink(const QString& existing_file, const QString& new_file)
    {
    #if defined(Q_OS_WIN)

        bool success = (0 != CreateHardLinkW(new_file.toStdWString().c_str(),
                                             existing_file.toStdWString().c_str(),
                                             NULL));
        if (not success)
            {
            DWORD ErrorMessageId = GetLastError();
            LPSTR message_buffer = nullptr;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                           FORMAT_MESSAGE_FROM_SYSTEM     | 
                           FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, 
                           ErrorMessageId, 
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                           (LPSTR) &message_buffer, 
                           0, 
                           NULL);
            qInfo() << QString("No success %1(%2,%3) for reason '%4'")
                       .arg(__PRETTY_FUNCTION__)
                       .arg(existing_file)
                       .arg(new_file)
                       .arg(message_buffer);
            LocalFree(message_buffer);
            }

    #elif defined(Q_OS_LINUX)

        bool success = (0 == link(qPrintable(existing_file), qPrintable(new_file)));
        if (not success)
            {
            qInfo() << QString("No success %1(%2,%3) for reason '%4'")
                       .arg(__PRETTY_FUNCTION__)
                       .arg(existing_file)
                       .arg(new_file)
                       .arg(strerror(errno)));
            }

    #else
        #error Unsupported OS.
    #endif
    return success;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

QString cb_sizestring_from_size(uint64_t size)
    {
    if (size > giga)
        {
        return QString("%1 GB").arg(static_cast <double> (size)/giga, 8, 'f', 2);
        }
    else if (size > mega)
        {
        return QString("%1 MB").arg(static_cast <double> (size)/mega, 8, 'f', 2);
        }
    else if (size > kilo)
        {
        return QString("%1 KB").arg(static_cast <double> (size)/kilo, 8, 'f', 2);
        }
    else
        {
        return QString("%1 B").arg(static_cast <double> (size)/kilo, 8, 'f', 0);
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

uint64_t cb_size_from_key(const QString& key)
    {
    return key.split("_").at(0).toULongLong();
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

QString cb_sizestring_from_key(const QString& key)
    {
    return cb_sizestring_from_size(cb_size_from_key(key));
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

QString cb_represent(const QStringList& string_list)
    {
    QString rv = "[";
    bool    need_separator = false;
    for (auto&& string : string_list) 
        {
        if (need_separator) 
            {
            rv += ", ";
            }
        rv += string;
        need_separator = true;
        }
    rv += "]";
    return rv;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
