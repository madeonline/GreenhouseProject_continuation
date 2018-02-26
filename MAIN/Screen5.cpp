//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screen5.h"
#include "DS3231.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Screen5::Screen5() : AbstractTFTScreen("SCREEN5")
{
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen5::doSetup(TFTMenu* menu)
{
	// тут настраиваемся, например, можем добавлять кнопки

	screenButtons->setSymbolFont(Various_Symbols_32x32);

	screenButtons->addButton(79, 25, 35, 35, "c",BUTTON_SYMBOL);
	screenButtons->addButton(127, 25, 35, 35, "c", BUTTON_SYMBOL);
	screenButtons->addButton(175, 25, 35, 35, "c", BUTTON_SYMBOL);
	screenButtons->addButton(79, 95, 35, 35, "d", BUTTON_SYMBOL);
	screenButtons->addButton(127, 95, 35, 35, "d", BUTTON_SYMBOL);
	screenButtons->addButton(175, 95, 35, 35, "d", BUTTON_SYMBOL);
	screenButtons->setTextFont(TFT_FONT);
	screenButtons->addButton(5, 145, 100, 30, "ВЫХОД");
	screenButtons->addButton(119, 145, 100, 30, "СОХР."); // кнопка Часы 

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen5::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen5::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  dc->setColor(VGA_WHITE);
  dc->setBackColor(VGA_BLACK);
  dc->setFont(BigRusFont);
 
 // получаем текущую дату
  DS3231Time dt = RealtimeClock.getTime();

  t_temp_date = dt.dayOfMonth;
  t_temp_mon = dt.month;
  t_temp_year = dt.year;
  t_temp_dow = dt.dayOfWeek;
  t_temp_hour = dt.hour;
  t_temp_min = dt.minute;
  t_temp_sec = dt.second;

  if (t_temp_date == 0)
  {
	  t_temp_date = 10;
	  t_temp_mon = 1;
	  t_temp_year = 2018;
	  t_temp_dow = 2;
	  t_temp_hour = 9;
	  t_temp_min = 0;
	  t_temp_sec = 0;
  }


  if (t_temp_hour<10)
  {
	  dc->printNumI(0, 81, 70);
	  dc->printNumI(t_temp_hour, 97, 70);
  }
  else
  {
	  dc->printNumI(t_temp_hour, 81, 70);
  }
  if (t_temp_min<10)
  {
	  dc->printNumI(0, 128, 70);
	  dc->printNumI(t_temp_min, 144, 70);
  }
  else
  {
	  dc->printNumI(t_temp_min, 128, 70);
  }
  if (t_temp_sec<10)
  {
	  dc->printNumI(0, 175, 70);
	  dc->printNumI(t_temp_sec, 191, 70);
  }
  else
  {
	  dc->printNumI(t_temp_sec, 175, 70);
  }
  
  // тут рисуем, что надо именно нам, кнопки прорисуются сами после того, как мы тут всё отрисуем
  menu->print("Установка", 35, 1);
  menu->print("Время", 0, 70);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen5::onButtonPressed(TFTMenu* menu, int pressedButton)
{
	UTFT* dc = menu->getDC();
	dc->setColor(VGA_WHITE);
	dc->setBackColor(VGA_BLACK);
  // обработчик нажатия на кнопку. Номера кнопок начинаются с 0 и идут в том порядке, в котором мы их добавляли
  if(pressedButton == 6)
    menu->switchToScreen("SCREEN4"); // переключаемся на первый экран
  else if (pressedButton == 7)
  {
	  RealtimeClock.setTime(t_temp_sec, t_temp_min, t_temp_hour, t_temp_dow, t_temp_date, t_temp_mon, t_temp_year);
  }
  else if (pressedButton == 0)
  {
	  t_temp_hour += 1;
	  if (t_temp_hour == 24)
		  t_temp_hour = 0;
	  if (t_temp_hour<10)
	  {
		  dc->printNumI(0, 81, 70);
		  dc->printNumI(t_temp_hour, 97, 70);
	  }
	  else
	  {
		  dc->printNumI(t_temp_hour, 81, 70);
	  }
  }
  else if (pressedButton == 1)
  {
	  t_temp_min += 1;
	  if (t_temp_min == 60)
		  t_temp_min = 0;
	  if (t_temp_min<10)
	  {
		  dc->printNumI(0, 128, 70);
		  dc->printNumI(t_temp_min, 144, 70);
	  }
	  else
	  {
		  dc->printNumI(t_temp_min, 128, 70);
	  }
  }
  else if (pressedButton == 2)
  {
	  t_temp_sec += 1;
	  if (t_temp_sec == 60)
		  t_temp_sec = 0;
	  if (t_temp_sec<10)
	  {
		  dc->printNumI(0, 175, 70);
		  dc->printNumI(t_temp_sec, 191, 70);
	  }
	  else
	  {
		  dc->printNumI(t_temp_sec, 175, 70);
	  }
  }
  else if (pressedButton == 3)
  {
	  t_temp_hour -= 1;
	  if (t_temp_hour < 0)
		  t_temp_hour = 23;
	  if (t_temp_hour<10)
	  {
		  dc->printNumI(0, 81, 70);
		  dc->printNumI(t_temp_hour, 97, 70);
	  }
	  else
	  {
		  dc->printNumI(t_temp_hour, 81, 70);
	  }
  }
  else if (pressedButton == 4)
  {
	  t_temp_min -= 1;
	  if (t_temp_min < 0)
		  t_temp_min = 59;
	  if (t_temp_min<10)
	  {
		  dc->printNumI(0, 128, 70);
		  dc->printNumI(t_temp_min, 144, 70);
	  }
	  else
	  {
		  dc->printNumI(t_temp_min, 128, 70);
	  }
  }
  else if (pressedButton == 5)
  {
	  t_temp_sec -= 1;
	  if (t_temp_sec < 0)
		  t_temp_sec = 59;
	  if (t_temp_sec<10)
	  {
		  dc->printNumI(0, 175, 70);
		  dc->printNumI(t_temp_sec, 191, 70);
	  }
	  else
	  {
		  dc->printNumI(t_temp_sec, 175, 70);
	  }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
