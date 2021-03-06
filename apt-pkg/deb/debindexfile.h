// -*- mode: cpp; mode: fold -*-
// Description								/*{{{*/
/* ######################################################################

   Debian Index Files
   
   There are three sorts currently
   
   Package files that have File: tags
   Package files that don't (/var/lib/dpkg/status)
   Source files
   
   ##################################################################### */
									/*}}}*/
#ifndef PKGLIB_DEBINDEXFILE_H
#define PKGLIB_DEBINDEXFILE_H

#include <apt-pkg/indexfile.h>
#include <apt-pkg/cacheiterators.h>
#include <apt-pkg/pkgcache.h>
#include <apt-pkg/srcrecords.h>

#include <string>

class OpProgress;
class pkgAcquire;
class pkgCacheGenerator;


class APT_HIDDEN debStatusIndex : public pkgIndexFile
{
   void *d;
   protected:
   std::string File;

   public:

   virtual const Type *GetType() const APT_CONST;

   // Interface for acquire
   virtual std::string Describe(bool /*Short*/) const {return File;};

   // Interface for the Cache Generator
   virtual bool Exists() const;
   virtual bool HasPackages() const {return true;};
   virtual unsigned long Size() const;
   virtual bool Merge(pkgCacheGenerator &Gen,OpProgress *Prog) const;
   virtual pkgCache::PkgFileIterator FindInCache(pkgCache &Cache) const;

   debStatusIndex(std::string File);
   virtual ~debStatusIndex();
};

class APT_HIDDEN debPackagesIndex : public pkgIndexTargetFile
{
   void *d;
   public:

   virtual const Type *GetType() const APT_CONST;

   // Stuff for accessing files on remote items
   virtual std::string ArchiveInfo(pkgCache::VerIterator Ver) const;

   // Interface for the Cache Generator
   virtual bool HasPackages() const {return true;};
   virtual bool Merge(pkgCacheGenerator &Gen,OpProgress *Prog) const;
   virtual pkgCache::PkgFileIterator FindInCache(pkgCache &Cache) const;

   debPackagesIndex(IndexTarget const &Target, bool const Trusted);
   virtual ~debPackagesIndex();
};

class APT_HIDDEN debTranslationsIndex : public pkgIndexTargetFile
{
   void *d;
   public:

   virtual const Type *GetType() const APT_CONST;

   // Interface for the Cache Generator
   virtual bool HasPackages() const;
   virtual bool Merge(pkgCacheGenerator &Gen,OpProgress *Prog) const;
   virtual pkgCache::PkgFileIterator FindInCache(pkgCache &Cache) const;

   debTranslationsIndex(IndexTarget const &Target);
   virtual ~debTranslationsIndex();
};

class APT_HIDDEN debSourcesIndex : public pkgIndexTargetFile
{
   void *d;
   public:

   virtual const Type *GetType() const APT_CONST;

   // Stuff for accessing files on remote items
   virtual std::string SourceInfo(pkgSrcRecords::Parser const &Record,
			     pkgSrcRecords::File const &File) const;

   // Interface for the record parsers
   virtual pkgSrcRecords::Parser *CreateSrcParser() const;

   // Interface for the Cache Generator
   virtual bool HasPackages() const {return false;};

   debSourcesIndex(IndexTarget const &Target, bool const Trusted);
   virtual ~debSourcesIndex();
};

class APT_HIDDEN debDebPkgFileIndex : public pkgIndexFile
{
 private:
   void *d;
   std::string DebFile;
   std::string DebFileFullPath;

 public:
   virtual const Type *GetType() const APT_CONST;

   virtual std::string Describe(bool /*Short*/) const {
      return DebFile;
   }

   /** get the control (file) content of the deb file
    *
    * @param[out] content of the control file
    * @param debfile is the filename of the .deb-file
    * @return \b true if successful, otherwise \b false.
    */
   static bool GetContent(std::ostream &content, std::string const &debfile);

   // Interface for the Cache Generator
   virtual bool Exists() const;
   virtual bool HasPackages() const {
      return true;
   };
   virtual unsigned long Size() const;
   virtual bool Merge(pkgCacheGenerator &Gen,OpProgress *Prog) const;
   virtual pkgCache::PkgFileIterator FindInCache(pkgCache &Cache) const;

   // Interface for acquire
   virtual std::string ArchiveURI(std::string /*File*/) const;

   debDebPkgFileIndex(std::string DebFile);
   virtual ~debDebPkgFileIndex();
};

class APT_HIDDEN debDscFileIndex : public pkgIndexFile
{
 private:
   void *d;
   std::string DscFile;
 public:
   virtual const Type *GetType() const APT_CONST;
   virtual pkgSrcRecords::Parser *CreateSrcParser() const;
   virtual bool Exists() const;
   virtual bool HasPackages() const {return false;};
   virtual unsigned long Size() const;
   virtual std::string Describe(bool /*Short*/) const {
      return DscFile;
   };

   debDscFileIndex(std::string &DscFile);
   virtual ~debDscFileIndex();
};

class APT_HIDDEN debDebianSourceDirIndex : public debDscFileIndex
{
 public:
   virtual const Type *GetType() const APT_CONST;
};

#endif
