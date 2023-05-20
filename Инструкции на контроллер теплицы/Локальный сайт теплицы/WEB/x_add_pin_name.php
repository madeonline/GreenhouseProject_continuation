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
   $pin_name = $dbengine->escapeString(mb_substr(strip_tags(@$_GET['name']),0,50));
    
    if($controller_id > 0 && $pin_name != '')
    {
      // проверяем, существует ли таблица имен пинов?
      createIfNotExists('pins_names',"CREATE TABLE pins_names (
    record_id     INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,
    controller_id INTEGER NOT NULL,
    pin_number    INTEGER DEFAULT (0),
    pin_name VARCHAR(50) 
);");
      
      // добавляем имя пина
      $dbengine->beginTransaction();
      $dbengine->exec("DELETE FROM pins_names WHERE controller_id=$controller_id AND pin_number=$pin_number;");
      $dbengine->exec("INSERT INTO pins_names(controller_id,pin_number, pin_name) VALUES($controller_id,$pin_number, '$pin_name');");
      $dbengine->commitTransaction();
    }
  
  
      
  } // if
 
}

// создаём массив данных
$json_data = array('authorized' => $authorized);

// отсылаем его юзеру
echo json_encode($json_data);

?>