<?php
// эта строчка обязательна к подключению всеми скриптами
require_once($_SERVER['DOCUMENT_ROOT'] . "/prerequisites.php");

$pins = array();

if($authorized)
{
  $posted = intval(@$_GET['posted']);
  if($posted == 1)
  {
   $controller_id = intval(@$_GET['controller_id']);
    
    if($controller_id > 0)
    {
      // проверяем, существует ли таблица пинов?
      createIfNotExists('excluded_pins',"CREATE TABLE excluded_pins (
    record_id     INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,
    controller_id INTEGER NOT NULL,
    pin_number    INTEGER DEFAULT (0) 
);");
      
// выбираем настройки из базы
      $res = $dbengine->query("SELECT * FROM excluded_pins WHERE controller_id=$controller_id ORDER BY pin_number;");
      if($res !== FALSE)
      { 
        while($array = $res->fetchArray())
        {
         $pins[] = $array;
        }
      }
            
    }
  
  
      
  } // if
 
}

// создаём массив данных
$json_data = array('authorized' => $authorized, 'pins' => $pins);

// отсылаем его юзеру
echo json_encode($json_data);

?>