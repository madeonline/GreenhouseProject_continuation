#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "UTFTMenu.h"
#include <SdFat.h>
#include "InterruptHandler.h"
#include "Drawing.h"
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

      int backButton, /*sdFormatButton, sdInfoButton, filesButton, ethalonButton*/ filesButton, sdScreenButton, systemScreenButton, paramsScreenButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class FilesScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new FilesScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      FilesScreen();

      int filesListButton, ethalonScreenButton, viewLogButton, clearDataButton, backButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class FilesListScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new FilesListScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      FilesListScreen();

      int viewChartsButton, backButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ClearDataScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new ClearDataScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      ClearDataScreen();

      int clearDataButton, backButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SDScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new SDScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      SDScreen();

      int sdInfoButton, formatSDButton, backButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SystemScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new SystemScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      SystemScreen();

      int communicateButton, backButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CommunicateScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new CommunicateScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      CommunicateScreen();

      int rs485Button, wiFiButton, backButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ParamsScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new ParamsScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      ParamsScreen();

      int inductiveSensorButton, transformerButton, backButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class InductiveSensorScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new InductiveSensorScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      InductiveSensorScreen();

      int pulsesCountButton, pulseDeltaButton, motoresourceButton, motoresourceMaxButton, backButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class PulsesCountScreen : public AbstractTFTScreen, public KeyboardInputTarget
{
  public:

  static AbstractTFTScreen* create()
  {
    return new PulsesCountScreen();
  }

  virtual void onKeyboardInput(bool enterPressed, const String& enteredValue);
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      PulsesCountScreen();

      int channel1Button, channel2Button, channel3Button, backButton;
      String channel1PulsesVal, channel2PulsesVal, channel3PulsesVal;
      int currentEditedButton;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class PulsesDeltaScreen : public AbstractTFTScreen, public KeyboardInputTarget
{
  public:

  static AbstractTFTScreen* create()
  {
    return new PulsesDeltaScreen();
  }

  virtual void onKeyboardInput(bool enterPressed, const String& enteredValue);
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      PulsesDeltaScreen();

      int channel1Button, channel2Button, channel3Button, backButton;
      String channel1DeltaVal, channel2DeltaVal, channel3DeltaVal;
      int currentEditedButton;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MotoresourceScreen : public AbstractTFTScreen, public KeyboardInputTarget
{
  public:

  static AbstractTFTScreen* create()
  {
    return new MotoresourceScreen();
  }

  virtual void onKeyboardInput(bool enterPressed, const String& enteredValue);
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);
    virtual void onActivate();

private:
      MotoresourceScreen();

      int channel1Button, channel2Button, channel3Button, backButton, resetButton;
      String channel1MotoresourceVal, channel2MotoresourceVal, channel3MotoresourceVal;
      int currentEditedButton;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MotoresourceMaxScreen : public AbstractTFTScreen, public KeyboardInputTarget
{
  public:

  static AbstractTFTScreen* create()
  {
    return new MotoresourceMaxScreen();
  }

  virtual void onKeyboardInput(bool enterPressed, const String& enteredValue);
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);
    virtual void onActivate();

private:
      MotoresourceMaxScreen();

      int channel1Button, channel2Button, channel3Button, backButton, resetButton;
      String channel1MotoresourceVal, channel2MotoresourceVal, channel3MotoresourceVal;
      int currentEditedButton;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TransformerScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new TransformerScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      TransformerScreen();

      int borderMaxButton, borderMinButton, backButton;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class BorderMaxScreen : public AbstractTFTScreen, public KeyboardInputTarget
{
  public:

  static AbstractTFTScreen* create()
  {
    return new BorderMaxScreen();
  }

  virtual void onKeyboardInput(bool enterPressed, const String& enteredValue);
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);
    virtual void onActivate();

private:
      BorderMaxScreen();

      int channel1Button, backButton, resetButton;
      String channel1BorderVal;
      int currentEditedButton;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class BorderMinScreen : public AbstractTFTScreen, public KeyboardInputTarget
{
  public:

  static AbstractTFTScreen* create()
  {
    return new BorderMinScreen();
  }

  virtual void onKeyboardInput(bool enterPressed, const String& enteredValue);
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);
    virtual void onActivate();

private:
      BorderMinScreen();

      int channel1Button, backButton, resetButton;
      String channel1BorderVal;
      int currentEditedButton;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class EthalonScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new EthalonScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      EthalonScreen();

      int viewEthalonButton, createEthalonButton, singleButton, ethalonFlagButton, backButton;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  recStarted,
  recDone
  
} EthalonRecordState;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  dirUp,
  dirDown
} EthalonDirection;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class EthalonRecordScreen : public AbstractTFTScreen, public InterruptEventSubscriber
{
  public:

  static AbstractTFTScreen* create()
  {
    return new EthalonRecordScreen();
  }

  void OnInterruptRaised(const InterruptTimeList& list, uint8_t listNum, EthalonCompareResult compareResult);
  void OnHaveInterruptData();
  void OnTimeBeforeInterruptsBegin(uint32_t tm, bool hasTime) {}
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);
    virtual void onActivate();
    virtual void onDeactivate();

private:
      EthalonRecordScreen();

      EthalonRecordState state;

      int8_t currentDrawState;
      void drawState(TFTMenu* menu);

      void drawWelcome(TFTMenu* menu);

      void resetButtons();

      EthalonDirection direction;
      void saveEthalons();
      void saveEthalon(int selChannel, int saveChannel);

      int backButton, saveButton, channel1Button, channel2Button, channel3Button, channel1SaveButton, channel2SaveButton, channel3SaveButton, directionButton;
      int channel1SelectedChannel, channel2SelectedChannel, channel3SelectedChannel;
      int channel1SaveChannel, channel2SaveChannel, channel3SaveChannel;
      void rotateSelectedChannel(int button, int& val);
      void showButtons(bool bShow);
  
      InterruptTimeList list1;
      InterruptTimeList list2;
      InterruptTimeList list3;
    
      Points serie1;
      Points serie2;
      Points serie3;
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class EthalonChartScreen : public AbstractTFTScreen
{
  public:

  static EthalonChartScreen* create()
  {
    return new EthalonChartScreen();
  }

  void show(const String& fName);
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      EthalonChartScreen();

      int backButton;
      String fileName;
      Points serie;
  
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
  String getName(const char* fileRootDir);
  uint32_t getTimestamp(const char* fileRootDir);
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SCREEN_FILES_COUNT 5 // кол-во файлов на одном экране
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum 
{
  vtLogsListing,
  vtEthalonsListing
  
} ListFilesType;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ListFilesScreen : public AbstractTFTScreen
{
  public:

  static ListFilesScreen* create(ListFilesType vt);

  void rescanFiles();
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      ListFilesScreen(ListFilesType vt, const char* name);
      ~ListFilesScreen();

      ListFilesType viewType;
      const char* getDirName();

      int backButton, viewFileButton;
      bool hasSD;

      int lastSelectedFileIndex;

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

