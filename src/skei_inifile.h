#ifndef skei_inifile_included
#define skei_inifile_included
//----------------------------------------------------------------------

#include "skei_array.h"
#include "skei_file.h"
#include "skei_parser.h"
#include "skei_string.h"

//----------------------------------------------------------------------

struct SIniItem {
  SString   name;
  SString   value;
  SIniItem(const char* AName, const char* AValue) {
    name = AName;
    value = AValue;
  }
};

typedef SArray<SIniItem*>    SIniItems;

//----------

struct SIniSection {
  SString   name;
  SIniItems items;
  SIniSection(const char* AName) {
    name = AName;
  }
};

typedef SArray<SIniSection*> SIniSections;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class SIniFile {

  private:

    SParser       MParser;
    SIniSections  MSections;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    SIniFile() {
    }

    //----------

    virtual ~SIniFile() {
      #ifndef SKEI_NO_AUTODELEE
      deleteSections();
      #endif
    }

  //----------------------------------------
  //
  //----------------------------------------

  private:

    void appendSection(SIniSection* ASection) {
      MSections.append(ASection);
    }

    //----------

    void deleteSections(void) {
      int32 num_sections = MSections.size();
      for (int32 i=0; i<num_sections; i++) {
        deleteItems( MSections[i] );
        delete MSections[i];
      }
    }

    //----------

    void appendItem(SIniSection* ASection, SIniItem* AItem) {
      ASection->items.append(AItem);
    }

    //----------

    void deleteItems(SIniSection* ASection) {
      for (int32 i=0; i<ASection->items.size(); i++) {
        delete ASection->items[i];
      }
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    const char* getStrItem(const char* ASection, const char* AItem) {
      int32 num_sections = MSections.size();
      for (int32 i=0; i<num_sections; i++) {
        SIniSection* section = MSections[i];
        const char* section_name = section->name.ptr();
        DTrace("section %i '%s'\n",i,section_name);
        if ( SStrcmp(section_name,ASection) == 0) {
          int32 num_items = MSections[i]->items.size();
          for (int32 j=0; j<num_items; j++) {
            SIniItem* item = MSections[i]->items[j];
            const char* item_name = item->name.ptr();
            const char* item_value = item->value.ptr();
            if ( SStrcmp(item_name,AItem) == 0) {
              return item_value;
            }
          }

        }
      }
      return SKEI_NULL;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    bool load(const char* AFilename) {
      // ----- load file -----
      SFile file(AFilename);
      if (!file.exists()) return false;
      file.open();
      int32 len = file.length();
      char* buffer = (char*)SMalloc(len);
      file.read(buffer,len);
      file.close();
      // ----- parse tokens -----
      MParser.parse(buffer,len);
      SFree(buffer);
      // ----- setup ini sections/items -----
      int32 numtokens = MParser.numTokens();
      int32 curtoken = 0;
      SIniSection* section;
      SIniItem* item;
      while (curtoken<numtokens) {
        const char* token = MParser.token(curtoken++);
        if (SStrchr(token,'#')) {
          const char* s = MParser.token(curtoken++);
          //DTrace("section: '%s'\n",section);
          section = new SIniSection(s);
          appendSection(section);
        }
        else {
          const char* i = token;
          const char* v = MParser.token(curtoken++);
          //DTrace("item: '%s' = '%s'\n",i,v);
          item = new SIniItem(i,v);
          appendItem(section,item);
        }
      }
      return true;
    }

    //----------

    bool save(const char* AFilename) {
      return false;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:


};

//----------------------------------------------------------------------
#endif
