#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "UTFTMenu.h"
#include <SdFat.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// экран номер 2
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Screen2 : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new Screen2();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      Screen2();

      int backButton, sdFormatButton, sdInfoButton, filesButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SDInfoScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new SDInfoScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      SDInfoScreen();

      int backButton;
      bool hasSD;

      void collectSDInfo();
      uint32_t cardSize, freeSpace;
      uint8_t fatType;

      String formatSize(uint32_t sz);
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SDFormatScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new SDFormatScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      SDFormatScreen();

      int backButton, formatButton, clearFatButton;
      bool hasSD;
     
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BU16 128
#define BU32 8192
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SDFormatter
{
  public:

  SDFormatter();
  void formatCard();
  void eraseCard();

  private:

      uint32_t cardSizeBlocks;
      uint32_t cardCapacityMB;
      uint8_t sectorsPerCluster;
      uint32_t fatStart;
      uint32_t fatSize;
      uint16_t reservedSectors;
      uint32_t dataStart;
      
      uint32_t partSize;
      uint8_t partType;
      uint32_t relSector;

      uint8_t numberOfHeads;
      uint8_t sectorsPerTrack;

      cache_t cache;

      
      bool initSizes();
      void makeFat16();
      void makeFat32();
      bool writeMbr();
      void clearCache(uint8_t addSig);
      uint16_t lbnToCylinder(uint32_t lbn);
      uint8_t lbnToHead(uint32_t lbn);
      uint8_t lbnToSector(uint32_t lbn);
      uint8_t writeCache(uint32_t lbn);
      uint32_t volSerialNumber();
      bool clearFatDir(uint32_t bgn, uint32_t count);

      void sdError(const char* msg);  
    
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SDFormatterScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new SDFormatterScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      SDFormatterScreen();  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SDEraserScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new SDEraserScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      SDEraserScreen();  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct FileEntry
{
  uint16_t dirIndex;
  String getName();
  uint32_t getTimestamp();
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SCREEN_FILES_COUNT 5 // кол-во файлов на одном экране
#define LOGS_DIRECTORY "/LOGS" // папка, в которой лежат логи
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SDFilesScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create();

  void rescanFiles();
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      SDFilesScreen();
      ~SDFilesScreen();

      int backButton;
      bool hasSD;

      int totalFilesCount;
      int totalPages;
      int currentPageNum;
      bool isFirstScan;
      String currentPageCaption;
      String filesNames[5];

      int currentPageButton, prevPageButton, nextPageButton;

      UTFT_Buttons_Rus* filesButtons;

      void drawCurrentPageNumber();

      FileEntry** files;
      void clearFiles();
      void sortFiles();

      void showPage(int step);
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

