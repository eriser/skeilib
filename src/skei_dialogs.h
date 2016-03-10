#ifndef skei_dialogs_included
#define skei_dialogs_included
//----------------------------------------------------------------------

// zenity: gnome?

//----------------------------------------------------------------------
// linux
//----------------------------------------------------------------------

#ifdef SKEI_LINUX

static char zenity_buffer[1024];
static char zenity_filename[1024];

//----------

const char* do_zenity(void) {
  FILE *f = popen(zenity_buffer,"r");
  if ( fgets(zenity_filename,1024,f) ) {
    return zenity_filename;
  }
  else {
    return "";//SKEI_NULL;
  }
}

//----------

const char* SDialog_ColorSelect(const char* ATitle, bool APalette=true) {
  SStrcpy(zenity_buffer,"zenity --color-selection");
  SStrcat(zenity_buffer," --title='");    SStrcat(zenity_buffer,ATitle);    SStrcat(zenity_buffer,"'");
  if (APalette) SStrcat(zenity_buffer," --show-palette");
  return do_zenity();
}

const char* SDialog_Error(const char* ATitle, const char* AText) {
  SStrcpy(zenity_buffer,"zenity --error");
  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
  return do_zenity();
}

const char* SDialog_FileOpen(const char* ATitle, bool AMultiple=false) {
  SStrcpy(zenity_buffer,"zenity --file-selection");
  SStrcat(zenity_buffer," --title='");    SStrcat(zenity_buffer,ATitle);    SStrcat(zenity_buffer,"'");
  if (AMultiple) {
    SStrcat(zenity_buffer," --multiple");
    SStrcat(zenity_buffer," --separator=':'");
  }
  return do_zenity();
}

const char* SDialog_FileSave(const char* ATitle, const char* AFilename) {
  SStrcpy(zenity_buffer,"zenity --file-selection");
  SStrcat(zenity_buffer," --save");
  SStrcat(zenity_buffer," --title='");    SStrcat(zenity_buffer,ATitle);    SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --filename='"); SStrcat(zenity_buffer,AFilename); SStrcat(zenity_buffer,"'");
  return do_zenity();
}

const char* SDialog_Info(const char* ATitle, const char* AText) {
  SStrcpy(zenity_buffer,"zenity --info");
  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
  return do_zenity();
}

const char* SDialog_Question(const char* ATitle, const char* AText) {
  SStrcpy(zenity_buffer,"zenity --question");
  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
  return do_zenity();
}

const char* SDialog_Text(const char* ATitle, const char* AText, const char* AEntryText) {
  SStrcpy(zenity_buffer,"zenity --entry");
  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);            SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --entry-text='");    SStrcat(zenity_buffer,AEntryText); SStrcat(zenity_buffer,"'");
  return do_zenity();
}

// segmentation fault

const char* SDialog_TextInfo(const char* ATitle, const char* AFilename, bool AEditable=false, bool AHtml=false, const char* AUrl=SKEI_NULL) {
  SStrcpy(zenity_buffer,"zenity --text-info");
  SStrcat(zenity_buffer," --filename='");       SStrcat(zenity_buffer,AFilename);   SStrcat(zenity_buffer,"'");
  if (AEditable) SStrcat(zenity_buffer," --editable");
  if (AHtml) SStrcat(zenity_buffer," --html");
  if (AUrl) SStrcat(zenity_buffer," --url='");  SStrcat(zenity_buffer,AUrl);        SStrcat(zenity_buffer,"'");
  return do_zenity();
}

const char* SDialog_Value(const char* ATitle, const char* AText, int AVal=0, int AMin=0, int AMax=100, int AStep=1) {
  char temp[256];
  SStrcpy(zenity_buffer,"zenity --scale");
  SStrcat(zenity_buffer," --text='");       SStrcat(zenity_buffer,AText); SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --value='");      SStrcat(zenity_buffer, SIntToString(temp,AVal));  SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --min-value='");  SStrcat(zenity_buffer, SIntToString(temp,AMin));  SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --max-value='");  SStrcat(zenity_buffer, SIntToString(temp,AMax));  SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --step='");       SStrcat(zenity_buffer, SIntToString(temp,AStep)); SStrcat(zenity_buffer,"'");
  //SStrcat(zenity_buffer," --print-partial");
  //SStrcat(zenity_buffer," --hide-value");
  return do_zenity();
}

const char* SDialog_Warning(const char* ATitle, const char* AText) {
  SStrcpy(zenity_buffer,"zenity --warning");
  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
  return do_zenity();
}

#endif // SKEI_LINUX

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------



//----------------------------------------------------------------------
#endif
