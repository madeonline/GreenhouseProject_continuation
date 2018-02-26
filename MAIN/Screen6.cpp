//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screen6.h"
#include "DS3231.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Screen6::Screen6() : AbstractTFTScreen("SCREEN6")
{
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen6::doSetup(TFTMenu* menu)
{
	// тут настраиваемся, например, можем добавлять кнопки

	screenButtons->setSymbolFont(Various_Symbols_32x32);

	screenButtons->addButton(74, 25, 35, 35, "c", BUTTON_SYMBOL);
	screenButtons->addButton(122, 25, 35, 35, "c", BUTTON_SYMBOL);
	screenButtons->addButton(170, 25, 35, 35, "c", BUTTON_SYMBOL);
	screenButtons->addButton(74, 95, 35, 35, "d", BUTTON_SYMBOL);
	screenButtons->addButton(122, 95, 35, 35, "d", BUTTON_SYMBOL);
	screenButtons->addButton(170, 95, 35, 35, "d", BUTTON_SYMBOL);
	screenButtons->setTextFont(TFT_FONT);
	screenButtons->addButton(5, 145, 100, 30, "ВЫХОД");
	screenButtons->addButton(119, 145, 100, 30, "СОХР."); // кнопка Часы 

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen6::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen6::doDraw(TFTMenu* menu)
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


  if (t_temp_date<10)
  {
	  dc->printNumI(0, 76, 70);
	  dc->printNumI(t_temp_date, 92, 70);
  }
  else
  {
	  dc->printNumI(t_temp_date, 76, 70);
  }
  if (t_temp_mon<10)
  {
	  dc->printNumI(0, 123, 70);
	  dc->printNumI(t_temp_mon, 139, 70);
  }
  else
  {
	  dc->printNumI(t_temp_mon, 123, 70);
  }
 
  dc->printNumI(t_temp_year - 2000, 170, 70);
 
 // dc->setFont(SmallRusFont);
  // тут рисуем, что надо именно нам, кнопки прорисуются сами после того, как мы тут всё отрисуем
  menu->print("Установка", 35, 1);
  menu->print("Дата", 2, 70);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen6::onButtonPressed(TFTMenu* menu, int pressedButton)
{
	UTFT* dc = menu->getDC();
	dc->setColor(VGA_WHITE);
	dc->setBackColor(VGA_BLACK);
	screenButtons->setTextFont(TFT_FONT);
	
	// обработчик нажатия на кнопку. Номера кнопок начинаются с 0 и идут в том порядке, в котором мы их добавляли
  if(pressedButton == 6)
    menu->switchToScreen("SCREEN4"); // переключаемся на первый экран
  else if (pressedButton == 7)
  {
	  RealtimeClock.setTime(t_temp_sec, t_temp_min, t_temp_hour, t_temp_dow, t_temp_date, t_temp_mon, t_temp_year);
  }
  else if (pressedButton == 0)
  {
	  t_temp_date += 1;
	  t_temp_date = validateDate(t_temp_date, t_temp_mon, t_temp_year);
	  if (t_temp_date<10)
	  {
		  dc->printNumI(0, 76, 70);
		  dc->printNumI(t_temp_date, 92, 70);
	  }
	  else
	  {
		  dc->printNumI(t_temp_date, 76, 70);
	  }
  }
  else if (pressedButton == 1)
  {
	  t_temp_mon += 1;
	  if (t_temp_mon == 13)
		  t_temp_mon = 1;
	  if (t_temp_mon<10)
	  {
		  dc->printNumI(0, 123, 70);
		  dc->printNumI(t_temp_mon, 139, 70);
	  }
	  else
	  {
		  dc->printNumI(t_temp_mon, 123, 70);
	  }
  }
  else if (pressedButton == 2)
  {
	  t_temp_year += 1;
	  if (t_temp_year >2099)
		  t_temp_year = 2099;
	  dc->printNumI(t_temp_year - 2000, 170, 70);
  }
  else if (pressedButton == 3)
  {
	  t_temp_date -= 1;
	  t_temp_date = validateDate(t_temp_date, t_temp_mon, t_temp_year);
	  if (t_temp_date<10)
	  {
		  dc->printNumI(0, 76, 70);
		  dc->printNumI(t_temp_date, 92, 70);
	  }
	  else
	  {
		  dc->printNumI(t_temp_date, 76, 70);
	  }

  }
  else if (pressedButton == 4)
  {
	  t_temp_mon -= 1;
	  if (t_temp_mon == 0)
		  t_temp_mon = 12;
	  if (t_temp_mon<10)
	  {
		  dc->printNumI(0, 123, 70);
		  dc->printNumI(t_temp_mon, 139, 70);
	  }
	  else
	  {
		  dc->printNumI(t_temp_mon, 123, 70);
	  }
  }
  else if (pressedButton == 5)
  {
	  t_temp_year -= 1;
	  if (t_temp_year < 2017)
		  t_temp_year = 2017;
	  dc->printNumI(t_temp_year - 2000, 170, 70);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte  Screen6::validateDate(byte d, byte m, word y)
{
	byte mArr[12] = { 31,0,31,30,31,30,31,31,30,31,30,31 };
	byte od;

	if (m == 2)
	{
		if ((y % 4) == 0)
		{
			if (d == 30)
				od = 1;
			else if (d == 0)
				od = 29;
			else
				od = d;
		}
		else
		{
			if (d == 29)
				od = 1;
			else if (d == 0)
				od = 28;
			else
				od = d;
		}
	}
	else
	{
		if (d == 0)
			od = mArr[m - 1];
		else if (d == (mArr[m - 1] + 1))
			od = 1;
		else
			od = d;
	}

	return od;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte  Screen6::validateDateForMonth(byte d, byte m, word y)
{
	byte mArr[12] = { 31,0,31,30,31,30,31,31,30,31,30,31 };
	byte od;
	//boolean dc = false;

	if (m == 2)
	{
		if ((y % 4) == 0)
		{
			if (d>29)
			{
				d = 29;
				//dc = true;
			}
		}
		else
		{
			if (d>28)
			{
				d = 28;
				//dc = true;
			}
		}
	}
	else
	{
		if (d>mArr[m - 1])
		{
			d = mArr[m - 1];
			//dc = true;
		}
	}

	return d;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

