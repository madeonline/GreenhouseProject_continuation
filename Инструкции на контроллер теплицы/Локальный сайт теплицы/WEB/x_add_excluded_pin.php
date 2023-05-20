<?php
// эта строчка обязательна к подключению всеми скриптами
require_once($_SERVER['DOCUMENT_ROOT'] . "/prerequisites.php");

if($authorized)
{
  $posted = intval(@$_GET['posted']);
  if($posted == 1)
  {
   $controller_id = intval(@$_GET['controller_id']);
   $pin_number = intval(@$_GET['pin']);
    
    if($controller_id > 0)
    {
      // проверяем, существует ли таблица пинов?
      createIfNotExists('excluded_pins',"CREATE TABLE excluded_pins (
    record_id     INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,
    controller_id INTEGER NOT NULL,
    pin_number    INTEGER DEFAULT (0) 
);");
      
      // добавляем пин в исключенные
      $dbengine->beginTransaction();
      $dbengine->exec("DELETE FROM excluded_pins WHERE controller_id=$controller_id AND pin_number=$pin_number;");
      $dbengine->exec("INSERT INTO excluded_pins(controller_id,pin_number) VALUES($controller_id,$pin_number);");
      $dbengine->commitTransaction();
    }
  
  
      
  } // if
 
}

// создаём массив данных
$json_data = array('authorized' => $authorized);

// отсылаем его юзеру
echo json_encode($json_data);

?>