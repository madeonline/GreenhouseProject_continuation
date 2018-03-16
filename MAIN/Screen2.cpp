//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screen2.h"
#include "FileUtils.h"
#include "TinyVector.h"
#include "CONFIG.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDFilesScreen* FilesScreen = NULL;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Screen2::Screen2() : AbstractTFTScreen("Settings")
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen2::doSetup(TFTMenu* menu)
{
  // тут настраиваемся, например, можем добавлять кнопки
  filesButton = screenButtons->addButton(5, 2, 210, 30, "Файлы");
  sdInfoButton = screenButtons->addButton(5, 37, 210, 30, "SD-инфо");
  sdFormatButton = screenButtons->addButton( 5, 72, 210, 30, "Формат SD");
  ethalonButton = screenButtons->addButton(5, 107, 210, 30, "Эталоны");
  backButton = screenButtons->addButton(5, 142, 210, 30, "ВЫХОД");

  // добавляем подэкраны работы с SD
  Screen.addScreen(SDInfoScreen::create());
  Screen.addScreen(SDFormatScreen::create());
  Screen.addScreen(SDFilesScreen::create());
  Screen.addScreen(EthalonScreen::create());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen2::doUpdate(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen2::doDraw(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen2::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("Main"); // переключаемся на главный экран
  else 
  if(pressedButton == sdInfoButton)
    menu->switchToScreen("SDInfo"); // переключаемся на экран информации о SD
  else 
  if(pressedButton == sdFormatButton)
    menu->switchToScreen("SDFormat"); // переключаемся на экран форматирования SD
  else 
  if(pressedButton == filesButton)
  {
    FilesScreen->rescanFiles();
    menu->switchToScreen(FilesScreen); // переключаемся на экран форматирования SD
  }
  else
  if(pressedButton == ethalonButton)
    menu->switchToScreen("Ethalon"); // переключаемся на экран работы с эталонами
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// EthalonScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EthalonScreen::EthalonScreen() : AbstractTFTScreen("Ethalon")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonScreen::doSetup(TFTMenu* menu)
{

  recordButton = screenButtons->addButton(5, 2, 210, 30, "Нов. эталон");
  backButton = screenButtons->addButton(5, 142, 210, 30, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("Settings"); // переключаемся на экран работы с SD
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FileEntry
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t FileEntry::getTimestamp()
{
  uint32_t result = 0;

  SdFile root, file;
  root.open(LOGS_DIRECTORY,O_READ);
  if(root.isOpen())
  {
    file.open(&root,dirIndex,O_READ);
    if(file.isOpen())
    {
      dir_t d;
      file.dirEntry(&d);
      result = (d.creationDate << 16) | d.creationTime;      
      file.close();
    }
    root.close();
  }

  return result;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String FileEntry::getName()
{
  String result;

  SdFile root, file;
  root.open(LOGS_DIRECTORY,O_READ);
  if(root.isOpen())
  {
    file.open(&root,dirIndex,O_READ);
    if(file.isOpen())
    {
      char nameBuff[50] = {0};
      file.getName(nameBuff,50);
      result = nameBuff;
      
      file.close();
    }
    root.close();
  }

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDFilesScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen* SDFilesScreen::create()
{
  if(FilesScreen)
    return FilesScreen;
    
  FilesScreen =  new SDFilesScreen();  
  return FilesScreen;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDFilesScreen::SDFilesScreen() : AbstractTFTScreen("SDFiles")
{
  filesButtons = NULL;
  totalFilesCount = 0;
  totalPages = 0;
  currentPageNum = 0;
  isFirstScan = true;
  currentPageButton = -1;
  files = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDFilesScreen::~SDFilesScreen()
{
  delete filesButtons;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFilesScreen::clearFiles()
{
  if(!files)
    return;
    
  for(int i=0;i<totalFilesCount;i++)
    delete files[i];

  delete [] files;
  files = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFilesScreen::sortFiles()
{
  if(!files)
    return;

  //TODO: Тут сортировка файлов!!!  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFilesScreen::showPage(int step)
{
  if(!files || !filesButtons)
    return;

   currentPageNum += step;
   if(currentPageNum < 0)
    currentPageNum = 0;

   if(currentPageNum >= totalPages)
    currentPageNum = totalPages - 1;

    // тут отображение файлов
    int startIndex = currentPageNum*SCREEN_FILES_COUNT;
    int endIndex = startIndex + SCREEN_FILES_COUNT;
    if(endIndex >  totalFilesCount)
      endIndex = totalFilesCount;

    int buttonCounter = 0;
    for(int i=startIndex;i<endIndex;i++)
    {
        FileEntry* entry = files[i];
        filesNames[buttonCounter] = entry->getName();
        
        filesButtons->relabelButton(buttonCounter,filesNames[buttonCounter].c_str());
        filesButtons->showButton(buttonCounter,isActive());
        
        buttonCounter++;
    } // buttonCounter

    // форсим перерисовку скрытых кнопок
    for(int i=buttonCounter;i<SCREEN_FILES_COUNT;i++)
    {
      filesButtons->hideButton(i,isActive());
    }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFilesScreen::rescanFiles()
{
  if(!hasSD)
    return;

   int lastFilesCount = totalFilesCount;
   totalFilesCount = FileUtils::CountFiles(LOGS_DIRECTORY);

   if(lastFilesCount != totalFilesCount)
   {
    clearFiles();

    files = new FileEntry*[totalFilesCount];

    SdFile file, root;
    
    for(int i=0;i<totalFilesCount;i++)
    {
      files[i] = new  FileEntry;
    } // for

    root.open(LOGS_DIRECTORY,O_READ);

    int cntr = 0;
    while (file.openNext(&root, O_READ)) 
    {
      if(cntr < totalFilesCount)
      {
        files[cntr]->dirIndex = file.dirIndex();
      }
      
      file.close();
      cntr++;
    }      
    root.close();

    sortFiles();
    
    // кол-во файлов изменилось!!!
    totalPages = totalFilesCount/SCREEN_FILES_COUNT;
    if(totalFilesCount % SCREEN_FILES_COUNT)
      totalPages++;

      if(isFirstScan)
      {
        isFirstScan = false;
        currentPageNum = totalPages-1;
      }
      
    //TODO: Тут перевычисление кол-ва страниц и вывод их в бокс, плюс - перерисовка экрана, если что-то изменилось

      if(filesButtons)
      {
        drawCurrentPageNumber();
      }
     
   } // if(lastFilesCount != totalFilesCount)
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFilesScreen::drawCurrentPageNumber()
{
  if(!isActive() || ! filesButtons || currentPageButton == -1)
    return;

    currentPageCaption = (currentPageNum + 1);
    currentPageCaption += '/';
    currentPageCaption += totalPages;

    filesButtons->relabelButton(currentPageButton,currentPageCaption.c_str(),true);
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFilesScreen::doSetup(TFTMenu* menu)
{
  // инициализируем SD
  hasSD = SDInit::InitSD();
    
  backButton = screenButtons->addButton(5, 142, 210, 30, "ВЫХОД");

  UTFT* dc = menu->getDC();
  int screenWidth = dc->getDisplayXSize();

  if(hasSD)
  {
    filesButtons = new UTFT_Buttons_Rus(menu->getDC(),menu->getTouch(),menu->getRusPrinter());
    filesButtons->setButtonColors(VGA_WHITE,VGA_WHITE,VGA_WHITE,VGA_RED,VGA_BLACK);
    filesButtons->setTextFont(SmallRusFont);
  
    int filesButtonHeight = 20;
    int filesButtonWidth = 120;
    int curY = 2;
    int curX = 5;
  
    for(int i=0;i<SCREEN_FILES_COUNT;i++)
    {
      int addedButton = filesButtons->addButton(curX, curY,filesButtonWidth,filesButtonHeight,"");
      filesButtons->hideButton(addedButton);
      
      curY += filesButtonHeight + 2;
    }


    curY += 2;
    int pageButtonWigth = 80;
    int navigationButtonWidth = 50;
    int pagesButtonLeft = (screenWidth - pageButtonWigth)/2;

    currentPageButton = filesButtons->addButton(pagesButtonLeft, curY,pageButtonWigth,filesButtonHeight,"", BUTTON_DISABLED);

    prevPageButton = filesButtons->addButton(5, curY,navigationButtonWidth,filesButtonHeight,"<");
    nextPageButton = filesButtons->addButton(screenWidth - navigationButtonWidth - 5, curY,navigationButtonWidth,filesButtonHeight,">");
    
  } // hasSD
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFilesScreen::doUpdate(TFTMenu* menu)
{
  if(filesButtons)
  {
    int checkedFilesButton = filesButtons->checkButtons();
    if(checkedFilesButton == prevPageButton)
    {
      showPage(-1);
      drawCurrentPageNumber();
    }
    else
    if(checkedFilesButton == nextPageButton)
    {
      showPage(1);
      drawCurrentPageNumber();
    }
    else if(checkedFilesButton == currentPageButton)
    {
      
    }
    else
    {
      //TODO: тут работаем с выбранным файлом!!!
      
    }
    
  } // if(filesButtons)

  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFilesScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);
  
  if(!hasSD)
  {
    // не удалось инициализировать!
    menu->print("Нет SD!", 10,10);
    dc->setFont(oldFont);
    return;
  }

  if(filesButtons)
  {
    filesButtons->drawButtons();
    drawCurrentPageNumber();
    
  } // if(filesButtons)

  showPage(0);

  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFilesScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("Settings"); // переключаемся на экран работы с SD  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDInfoScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDInfoScreen::SDInfoScreen() : AbstractTFTScreen("SDInfo")
{
  cardSize = 0;
  fatType = 0;
  freeSpace = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInfoScreen::doSetup(TFTMenu* menu)
{
  // инициализируем SD
  hasSD = SDInit::InitSD();
  
  backButton = screenButtons->addButton(5, 142, 210, 30, "ВЫХОД");

  // собираем информацию об SD
  collectSDInfo();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInfoScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInfoScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);
  
  if(!hasSD)
  {
    // не удалось инициализировать!
    menu->print("Нет SD!", 10,10);
    dc->setFont(oldFont);
    return;
  }

   int fontHeight = dc->getFontYsize();

   int curX = 10;
   int curY = 10;
   int lineSpacing = 2;
   
   String info = "Р-р: ";
   info += formatSize(cardSize);
   
   menu->print(info.c_str(), curX,curY);
   curY += fontHeight + lineSpacing;

   info = "Тип: FAT";
   info += fatType;

   menu->print(info.c_str(), curX,curY);
   curY += fontHeight + lineSpacing;

   info = "Своб.: ";
   info +=  formatSize(freeSpace);
   
   menu->print(info.c_str(), curX,curY);
   curY += fontHeight + lineSpacing;


   dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInfoScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("Settings"); // переключаемся на экран работы с SD
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInfoScreen::collectSDInfo()
{
  if(!hasSD)
    return;
    
  cardSize = SD.card()->cardSize();
  cardSize *= 512;
  
  fatType = SD.vol()->fatType();

  uint32_t volFree = SD.vol()->freeClusterCount();
  freeSpace = 512L*volFree*SD.vol()->blocksPerCluster();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String SDInfoScreen::formatSize(uint32_t sz)
{
   String result;

   if(sz < 1024L) // under 1 Kb
   {
      result = sz;
      result += "B";
      
   }
   else 
   if(sz < 1024L*1024L) // under 1 Mb
   {
      float f = (sz*1.0)/1024L;
      result += String(f,2);
      result += "Kb";
   }
   else 
   if(sz < 1024L*1024L*1024L) // under 1 Gb
   {
      float f = (sz*1.0)/(1024L*1024L);
      result += String(f,2);
      result += "Mb";
   }
   else // over 1 Gb
   {
      float f = (sz*1.0)/(1024L*1024L*1024L);
      result += String(f,2);
      result += "Gb";
   }

   return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDFormatScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDFormatScreen::SDFormatScreen() : AbstractTFTScreen("SDFormat")
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatScreen::doSetup(TFTMenu* menu)
{
  // инициализируем SD
  hasSD = SDInit::InitSD();

  if(hasSD)
  {
    formatButton = screenButtons->addButton( 5, 2, 210, 30, "Формат");
    clearFatButton = screenButtons->addButton(5, 37, 210, 30, "Стереть всё");
  }
  
  backButton = screenButtons->addButton(5, 142, 210, 30, "ВЫХОД");

  // добавляем экран форматирования
  menu->addScreen(SDFormatterScreen::create());  
  // добавляем экран очистки
  menu->addScreen(SDEraserScreen::create());  

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatScreen::doUpdate(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);
  
  if(!hasSD)
  {
    // не удалось инициализировать!
    menu->print("Нет SD!", 10,10);
    dc->setFont(oldFont);
    return;
  }

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("Settings"); // переключаемся на экран работы с SD
  else
  if(pressedButton == formatButton)
  {
    Vector<const char*> lines;
    lines.push_back("Начать");
    lines.push_back("формат?");    
    MessageBox->confirm(lines,"SDFormatter","SDFormat");
  }
  else
  if(pressedButton == clearFatButton)
  {
    Vector<const char*> lines;
    lines.push_back("Начать");
    lines.push_back("очистку?");    
    MessageBox->confirm(lines,"SDEraser","SDFormat");
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDFormatterScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDFormatterScreen::SDFormatterScreen() : AbstractTFTScreen("SDFormatter")
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatterScreen::doSetup(TFTMenu* menu)
{


}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatterScreen::doUpdate(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);

   int fontHeight = dc->getFontYsize();
   int fontWidth = dc->getFontXsize();
   int displayWidth = dc->getDisplayXSize();
   int displayHeight = dc->getDisplayYSize();
   int lineSpacing = 2;

   String message = "Идёт формат,";
   
   int lineLength = menu->print(message.c_str(),0,0,0,true);
   int curX = (displayWidth - lineLength*fontWidth)/2;
   int curY = (displayHeight - fontHeight*2 - lineSpacing)/2;

   menu->print(message.c_str(),curX,curY);  
   curY += fontHeight + lineSpacing;

   message = "ждите...";
   lineLength = menu->print(message.c_str(),0,0,0,true);
   curX = (displayWidth - lineLength*fontWidth)/2;
   menu->print(message.c_str(),curX,curY);  
     
  dc->setFont(oldFont);

    // форматируем карточку
    SDFormatter fmt;
    fmt.formatCard();

    Vector<const char*> lines;
    lines.push_back("Формат");
    lines.push_back("завершён.");    
    MessageBox->show(lines,"SDFormat");


}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatterScreen::doDraw(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatterScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDEraserScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDEraserScreen::SDEraserScreen() : AbstractTFTScreen("SDEraser")
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDEraserScreen::doSetup(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDEraserScreen::doUpdate(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);

   int fontHeight = dc->getFontYsize();
   int fontWidth = dc->getFontXsize();
   int displayWidth = dc->getDisplayXSize();
   int displayHeight = dc->getDisplayYSize();
   int lineSpacing = 2;

   String message = "Идёт очистка,";
   
   int lineLength = menu->print(message.c_str(),0,0,0,true);
   int curX = (displayWidth - lineLength*fontWidth)/2;
   int curY = (displayHeight - fontHeight*2 - lineSpacing)/2;

   menu->print(message.c_str(),curX,curY);  
   curY += fontHeight + lineSpacing;

   message = "ждите...";
   lineLength = menu->print(message.c_str(),0,0,0,true);
   curX = (displayWidth - lineLength*fontWidth)/2;
   menu->print(message.c_str(),curX,curY);  
     
  dc->setFont(oldFont);

    // форматируем карточку
    SDFormatter fmt;
    fmt.eraseCard();
    fmt.formatCard();

    Vector<const char*> lines;
    lines.push_back("Очистка");
    lines.push_back("завершена.");    
    MessageBox->show(lines,"SDFormat");


}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDEraserScreen::doDraw(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDEraserScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDFormatter
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDFormatter::SDFormatter()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::formatCard()
{
  if(initSizes())
  {  
    if(SD.card()->type() != SD_CARD_TYPE_SDHC)
      makeFat16();
    else
      makeFat32();    
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::sdError(const char* msg)
{
  DBG(F("SD ERROR: "));
  DBGLN(msg);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SDFormatter::writeMbr()
{
  clearCache(true);
  part_t* p = cache.mbr.part;
  p->boot = 0;
  uint16_t c = lbnToCylinder(relSector);
  if (c > 1023) 
  {
    sdError("MBR CHS");
    return false;
  }
  p->beginCylinderHigh = c >> 8;
  p->beginCylinderLow = c & 0XFF;
  p->beginHead = lbnToHead(relSector);
  p->beginSector = lbnToSector(relSector);
  p->type = partType;
  uint32_t endLbn = relSector + partSize - 1;
  c = lbnToCylinder(endLbn);
  if (c <= 1023) 
  {
    p->endCylinderHigh = c >> 8;
    p->endCylinderLow = c & 0XFF;
    p->endHead = lbnToHead(endLbn);
    p->endSector = lbnToSector(endLbn);
  } 
  else 
  {
    // Too big flag, c = 1023, h = 254, s = 63
    p->endCylinderHigh = 3;
    p->endCylinderLow = 255;
    p->endHead = 254;
    p->endSector = 63;
  }
  p->firstSector = relSector;
  p->totalSectors = partSize;
  if (!writeCache(0)) 
  {
    sdError("write MBR");
    return false;
  }  

  return true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t SDFormatter::lbnToSector(uint32_t lbn) 
{
  return (lbn % sectorsPerTrack) + 1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t SDFormatter::lbnToHead(uint32_t lbn) 
{
  return (lbn % (numberOfHeads * sectorsPerTrack)) / sectorsPerTrack;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t SDFormatter::lbnToCylinder(uint32_t lbn) 
{
  return lbn / (numberOfHeads * sectorsPerTrack);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::clearCache(uint8_t addSig) 
{
  memset(&cache, 0, sizeof(cache));
  if (addSig) 
  {
    cache.mbr.mbrSig0 = BOOTSIG0;
    cache.mbr.mbrSig1 = BOOTSIG1;
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::makeFat16()
{
  uint32_t nc;
  for (dataStart = 2 * BU16;; dataStart += BU16) 
  {
    nc = (cardSizeBlocks - dataStart)/sectorsPerCluster;
    fatSize = (nc + 2 + 255)/256;
    uint32_t r = BU16 + 1 + 2 * fatSize + 32;
    if (dataStart < r) 
    {
      continue;
    }
    relSector = dataStart - r + BU16;
    break;
  }
  // check valid cluster count for FAT16 volume
  if (nc < 4085 || nc >= 65525) 
  {
    sdError("Bad cluster count");
    return;
  }
  reservedSectors = 1;
  fatStart = relSector + reservedSectors;
  partSize = nc * sectorsPerCluster + 2 * fatSize + reservedSectors + 32;
  if (partSize < 32680) 
  {
    partType = 0X01;
  } 
  else if (partSize < 65536) 
  {
    partType = 0X04;
  } 
  else 
  {
    partType = 0X06;
  }
  
  // write MBR
  if(writeMbr())
  {
      clearCache(true);
      
      fat_boot_t* pb = &cache.fbs;
      pb->jump[0] = 0XEB;
      pb->jump[1] = 0X00;
      pb->jump[2] = 0X90;
      
      for (uint8_t i = 0; i < sizeof(pb->oemId); i++) 
      {
        pb->oemId[i] = ' ';
      }
      
      pb->bytesPerSector = 512;
      pb->sectorsPerCluster = sectorsPerCluster;
      pb->reservedSectorCount = reservedSectors;
      pb->fatCount = 2;
      pb->rootDirEntryCount = 512;
      pb->mediaType = 0XF8;
      pb->sectorsPerFat16 = fatSize;
      pb->sectorsPerTrack = sectorsPerTrack;
      pb->headCount = numberOfHeads;
      pb->hidddenSectors = relSector;
      pb->totalSectors32 = partSize;
      pb->driveNumber = 0X80;
      pb->bootSignature = EXTENDED_BOOT_SIG;
      pb->volumeSerialNumber = volSerialNumber();
      memcpy(pb->volumeLabel, "NO NAME    ", sizeof(pb->volumeLabel));
      memcpy(pb->fileSystemType, "FAT16   ", sizeof(pb->fileSystemType));
      
      // write partition boot sector
      if (!writeCache(relSector)) 
      {
        sdError("FAT16 write PBS failed");
        return;
      }
      // clear FAT and root directory
      if(clearFatDir(fatStart, dataStart - fatStart))
      {
        clearCache(false);
        cache.fat16[0] = 0XFFF8;
        cache.fat16[1] = 0XFFFF;
        // write first block of FAT and backup for reserved clusters
        if (!writeCache(fatStart)
            || !writeCache(fatStart + fatSize)) 
        {
          sdError("FAT16 reserve failed");
        }
      }
  } // if(writeMbr
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SDFormatter::clearFatDir(uint32_t bgn, uint32_t count) 
{
  clearCache(false);

  if (!SD.card()->writeStart(bgn, count)) 
  {
    sdError("Clear FAT/DIR writeStart failed");
    return false;
  }
  for (uint32_t i = 0; i < count; i++) 
  {
    if ((i & 0XFF) == 0) 
    {
      //  cout << '.';
    }
    if (!SD.card()->writeData(cache.data)) 
    {
      sdError("Clear FAT/DIR writeData failed");
      return false;
    }
  }
  if (!SD.card()->writeStop()) 
  {
    sdError("Clear FAT/DIR writeStop failed");
    return false;
  }

  return true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t SDFormatter::volSerialNumber() 
{
  return (cardSizeBlocks << 8) + micros();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t SDFormatter::writeCache(uint32_t lbn) 
{
  return SD.card()->writeBlock(lbn, cache.data);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::makeFat32()
{
  uint32_t nc;
  relSector = BU32;
  for (dataStart = 2 * BU32;; dataStart += BU32) 
  {
    nc = (cardSizeBlocks - dataStart)/sectorsPerCluster;
    fatSize = (nc + 2 + 127)/128;
    uint32_t r = relSector + 9 + 2 * fatSize;
    if (dataStart >= r) 
    {
      break;
    }
  }
  // error if too few clusters in FAT32 volume
  if (nc < 65525) 
  {
    sdError("Bad cluster count");
    return;
  }
  reservedSectors = dataStart - relSector - 2 * fatSize;
  fatStart = relSector + reservedSectors;
  partSize = nc * sectorsPerCluster + dataStart - relSector;
  // type depends on address of end sector
  // max CHS has lbn = 16450560 = 1024*255*63
  if ((relSector + partSize) <= 16450560) 
  {
    // FAT32
    partType = 0X0B;
  } 
  else 
  {
    // FAT32 with INT 13
    partType = 0X0C;
  }
  
  if(!writeMbr())
    return;
    
  clearCache(true);

  fat32_boot_t* pb = &cache.fbs32;
  pb->jump[0] = 0XEB;
  pb->jump[1] = 0X00;
  pb->jump[2] = 0X90;
  
  for (uint8_t i = 0; i < sizeof(pb->oemId); i++) 
  {
    pb->oemId[i] = ' ';
  }
  pb->bytesPerSector = 512;
  pb->sectorsPerCluster = sectorsPerCluster;
  pb->reservedSectorCount = reservedSectors;
  pb->fatCount = 2;
  pb->mediaType = 0XF8;
  pb->sectorsPerTrack = sectorsPerTrack;
  pb->headCount = numberOfHeads;
  pb->hidddenSectors = relSector;
  pb->totalSectors32 = partSize;
  pb->sectorsPerFat32 = fatSize;
  pb->fat32RootCluster = 2;
  pb->fat32FSInfo = 1;
  pb->fat32BackBootBlock = 6;
  pb->driveNumber = 0X80;
  pb->bootSignature = EXTENDED_BOOT_SIG;
  pb->volumeSerialNumber = volSerialNumber();
  memcpy(pb->volumeLabel, "NO NAME    ", sizeof(pb->volumeLabel));
  memcpy(pb->fileSystemType, "FAT32   ", sizeof(pb->fileSystemType));
  // write partition boot sector and backup
  if (!writeCache(relSector)
      || !writeCache(relSector + 6)) 
  {
    sdError("FAT32 write PBS failed");
    return;
  }
  clearCache(true);
  // write extra boot area and backup
  if (!writeCache(relSector + 2)
      || !writeCache(relSector + 8)) 
  {
    sdError("FAT32 PBS ext failed");
    return;
  }
  fat32_fsinfo_t* pf = &cache.fsinfo;
  pf->leadSignature = FSINFO_LEAD_SIG;
  pf->structSignature = FSINFO_STRUCT_SIG;
  pf->freeCount = 0XFFFFFFFF;
  pf->nextFree = 0XFFFFFFFF;
  // write FSINFO sector and backup
  if (!writeCache(relSector + 1)
      || !writeCache(relSector + 7)) 
  {
    sdError("FAT32 FSINFO failed");
    return;
  }
  
  if(!clearFatDir(fatStart, 2 * fatSize + sectorsPerCluster))
    return;
    
  clearCache(false);
  
  cache.fat32[0] = 0x0FFFFFF8;
  cache.fat32[1] = 0x0FFFFFFF;
  cache.fat32[2] = 0x0FFFFFFF;
  // write first block of FAT and backup for reserved clusters
  if (!writeCache(fatStart)
      || !writeCache(fatStart + fatSize)) 
  {
    sdError("FAT32 reserve failed");
  }  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SDFormatter::initSizes()
{
  cardSizeBlocks = SD.card()->cardSize();
  cardCapacityMB = (cardSizeBlocks + 2047)/2048;  

 if (cardCapacityMB <= 6) 
 {
    sdError("Card is too small.");
    return false;
  } 
  else if (cardCapacityMB <= 16) 
  {
    sectorsPerCluster = 2;
  } 
  else if (cardCapacityMB <= 32) 
  {
    sectorsPerCluster = 4;
  } 
  else if (cardCapacityMB <= 64) 
  {
    sectorsPerCluster = 8;
  } 
  else if (cardCapacityMB <= 128) 
  {
    sectorsPerCluster = 16;
  } 
  else if (cardCapacityMB <= 1024) 
  {
    sectorsPerCluster = 32;
  } 
  else if (cardCapacityMB <= 32768) 
  {
    sectorsPerCluster = 64;
  } 
  else 
  {
    // SDXC cards
    sectorsPerCluster = 128;
  }

  // set fake disk geometry
  sectorsPerTrack = cardCapacityMB <= 256 ? 32 : 63;

  if (cardCapacityMB <= 16) 
  {
    numberOfHeads = 2;
  } 
  else if (cardCapacityMB <= 32) 
  {
    numberOfHeads = 4;
  } 
  else if (cardCapacityMB <= 128) 
  {
    numberOfHeads = 8;
  } 
  else if (cardCapacityMB <= 504) 
  {
    numberOfHeads = 16;
  } 
  else if (cardCapacityMB <= 1008) 
  {
    numberOfHeads = 32;
  } 
  else if (cardCapacityMB <= 2016) 
  {
    numberOfHeads = 64;
  } 
  else if (cardCapacityMB <= 4032) 
  {
    numberOfHeads = 128;
  } 
  else 
  {
    numberOfHeads = 255;
  }
  return true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::eraseCard()
{
  if(!initSizes())
    return;
    
  uint32_t const ERASE_SIZE = 262144L;
  uint32_t firstBlock = 0;
  uint32_t lastBlock;
 
  do 
  {
    lastBlock = firstBlock + ERASE_SIZE - 1;
    if (lastBlock >= cardSizeBlocks) 
    {
      lastBlock = cardSizeBlocks - 1;
    }
    if (!SD.card()->erase(firstBlock, lastBlock)) 
    {
      sdError("erase failed");
      return;
    }
    firstBlock += ERASE_SIZE;
  } while (firstBlock < cardSizeBlocks);

  if (!SD.card()->readBlock(0, cache.data)) 
  {
    sdError("readBlock");
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------


