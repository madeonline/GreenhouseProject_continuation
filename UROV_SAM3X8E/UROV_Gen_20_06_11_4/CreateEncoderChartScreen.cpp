//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "CreateEncoderChartScreen.h"
#include "Drawing.h"
#include "Buzzer.h"

int point_X[10] = { 0 };
int point_Y[10] = { 0 };
int pointF_X[10] = { 0 };
int pointF_Y[10] = { 0 };

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CreateEncoderChartScreen::CreateEncoderChartScreen() : AbstractTFTScreen("CreateEncoderChartScreen")
{
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CreateEncoderChartScreen::doSetup(TFTMenu* menu)
{
  // тут настраиваемся, например, можем добавлять кнопки

	
	int menu_height = 30;
	int button_gap = 5;
	int height_button = 29;
	int width_button = 125;

  clearButton = screenButtons->addButton(5, 255, 150, 40, "ОЧИСТИТЬ");
  calculateButton = screenButtons->addButton(160, 255, 150, 40, "ВЫЧИСЛИТЬ");
  backButton = screenButtons->addButton( 315 ,  255, 150,  40, "ЗАВЕРШИТЬ");

  file1Button = screenButtons->addButton(340, menu_height, width_button, height_button, "FILE 1");
  menu_height += height_button + button_gap;
  file2Button = screenButtons->addButton(340, menu_height, width_button, height_button, "FILE 2");
  menu_height += height_button + button_gap;
  file3Button = screenButtons->addButton(340, menu_height, width_button, height_button, "FILE 3");
  menu_height += height_button + button_gap;
  mem1Button = screenButtons->addButton(340, menu_height, width_button, height_button, "MEMO 1");
  menu_height += height_button + button_gap;
  mem2Button = screenButtons->addButton(340, menu_height, width_button, height_button, "MEMO 2");
  menu_height += height_button + button_gap;
  mem3Button = screenButtons->addButton(340, menu_height, width_button, height_button, "MEMO 3");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CreateEncoderChartScreen::doUpdate(TFTMenu* menu)
{
	if (step_pount < max_step_pount)
	{
		get_Point_Screen(menu); // Определить точку на сетке

	}// 

    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CreateEncoderChartScreen::doDraw(TFTMenu* menu)
{

  UTFT* dc = menu->getDC();
  dc->setColor(VGA_WHITE);
  dc->setBackColor(VGA_BLACK);

  dc->setFont(BigRusFont);
  // тут рисуем, что надо именно нам, кнопки прорисуются сами после того, как мы тут всё отрисуем
  menu->print("Экран создания графика", 70, 10);
  dc->setFont(SmallRusFont);

  // рисуем сетку
  RGBColor gridColor = { 0,200,0 }; // цвет сетки
  int gridX = 20; // стартовая координата по X для сетки
  int gridY = 30; // стартовая координата по Y для сетки
  int columnsCount = 6; // количество столбцов сетки
  int rowsCount = 4; // количество строк сетки
  int columnWidth = 50; // ширина столбца
  int rowHeight = 50; // высота строки
  
  Drawing::DrawGrid(gridX, gridY, columnsCount, rowsCount, columnWidth, rowHeight, gridColor);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CreateEncoderChartScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
	// обработчик нажатия на кнопку.

	if (pressedButton == backButton)
	{
	   menu->switchToScreen("Main"); // переключаемся на первый экран
    }
    else if (pressedButton == clearButton)
    {
		clear_Grid(menu);  //  "ОЧИСТИТЬ"
	}
	else if (pressedButton == calculateButton)
	{
		create_Schedule(menu);  //  Сформировать график
	}
	else if (pressedButton == file1Button)
	{
		// Сохранить в файл 1
	}
	else if (pressedButton == file2Button)
	{
		// Сохранить в файл 2
	}
	else if (pressedButton == file3Button)
	{
		// Сохранить в файл 3
	}
	else if (pressedButton == mem1Button)
	{
		// Сохранить в память 1
	}
	else if (pressedButton == mem2Button)
	{
		// Сохранить в память 2
	}
	else if (pressedButton == mem3Button)
	{
		// Сохранить в память 3
	}
	else if (pressedButton == grid_Button)
	{
		Serial.println("pressedButton");// 
	}
	

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  CreateEncoderChartScreen::get_Point_Screen(TFTMenu* menu)
{
	URTouch* tftTouch_point = menu->getTouch();
	UTFT* dc = menu->getDC();
	dc->setColor(VGA_WHITE);
	dc->setBackColor(VGA_BLACK);

	if (tftTouch_point->dataAvailable() == true)
	{
		tftTouch_point->read();
		int		result = -1;
		int		touch_x = tftTouch_point->getX();
		int		touch_y = tftTouch_point->getY();

		step_pount++; // Следующая точка

		pointF_X[step_pount] = touch_x; // фактическая точко по X
		pointF_Y[step_pount] = touch_y; // фактическая точко по Y


		if ((touch_x > touch_x_min && touch_x < 321) && (touch_y > 30 && touch_y < 231)) //Вычислять только в пределах сетки с увеличением по Х
		{

			menu->print("*", touch_x-2, touch_y - 3);  // Точка на графике
			touch_x_min = touch_x;
			menu->print(">", touch_x_min, 236);        // стрелка ограничения по Х на графике
			dc->drawLine(20, 240, touch_x_min, 240);   // стрелка ограничения по Х на графике
			dc->setFont(BigRusFont);
			String stringVar = String(step_pount, DEC);
			menu->print(stringVar.c_str(), 320, 234);         // стрелка ограничения по Х на графике
			dc->setFont(SmallRusFont);
			touch_x -= 20;
			touch_y = 230 - touch_y;
			Serial.print("touch_x : ");
			Serial.print(touch_x);
			Serial.print(", touch_y : ");
			Serial.println(touch_y);
		}
		Buzzer.buzz();
		while (tftTouch_point->dataAvailable() == true) {}
		//delay(150);
		while (tftTouch_point->dataAvailable() == false) {}
		delay(150);
		//while (tftTouch_point->dataAvailable() == true) {}
		
		point_X[step_pount] = touch_x;
		point_Y[step_pount] = touch_y;
	}

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CreateEncoderChartScreen::clear_Grid(TFTMenu* menu)
{
	UTFT* dc = menu->getDC();
	dc->setColor(VGA_WHITE);
	dc->setBackColor(VGA_BLACK);
	dc->setFont(BigRusFont);
	// тут рисуем, что надо именно нам, кнопки прорисуются сами после того, как мы тут всё отрисуем
	menu->print("Экран создания графика", 70, 10);
	dc->setFont(SmallRusFont);

	// рисуем сетку
	RGBColor gridColor = { 0,200,0 }; // цвет сетки
	int gridX = 20;                   // стартовая координата по X для сетки
	int gridY = 30;                   // стартовая координата по Y для сетки
	int columnsCount = 6;             // количество столбцов сетки
	int rowsCount = 4;                // количество строк сетки
	int columnWidth = 50;             // ширина столбца
	int rowHeight = 50;               // высота строки
	touch_x_min = 20;                 // В начало графика

	dc->setColor(VGA_BLACK);
	dc->fillRect(gridX, gridY, gridX + 15 + (columnWidth*columnsCount), gridY + 18 + (rowHeight*rowsCount)); // Очистить экран
	Drawing::DrawGrid(gridX, gridY, columnsCount, rowsCount, columnWidth, rowHeight, gridColor); // Нарисовать новую сетку

	step_pount = 0;    //  Количество точек в начало

	for (int i = 0; i < max_step_pount+1; i++) // Очистить массив с данными по X,Y
	{
		point_X[i] = 0;
		point_Y[i] = 0;
		pointF_X[i] = 0;
		pointF_Y[i] = 0;
	}


} 
void CreateEncoderChartScreen::create_Schedule(TFTMenu* menu)  //  Сформировать график
{
	UTFT* dc = menu->getDC();
	dc->setColor(VGA_WHITE);
	dc->setBackColor(VGA_BLACK);
	pointF_X[0] = 20;
	pointF_X[max_step_pount + 1] = 320;
	pointF_Y[0] = 230;
	pointF_Y[max_step_pount + 1] = 230;

	//dc->drawLine(point_X[0] + 20, pointF_Y[0], point_X[i + 1], pointF_Y[i + 1]);

	for (int i = 0; i < max_step_pount+1; i++)
	{

		dc->drawLine(pointF_X[i], pointF_Y[i], pointF_X[i+1], pointF_Y[i+1]);

	}




}