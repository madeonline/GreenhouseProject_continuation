<?php
// эта строчка обязательна к подключению всеми скриптами
require_once($_SERVER['DOCUMENT_ROOT'] . "/prerequisites.php");

$result = false;

if($authorized)
{
  $posted = intval(@$_GET['posted']);
  if($posted == 1)
  {
   $controller_id = intval(@$_GET['controller_id']);
  

      // обновляем базу
      $dbengine->beginTransaction();
      $result = $dbengine->exec("DELETE FROM controllers WHERE controller_id=$controller_id;");
      $dbengine->commitTransaction();

    
      
  } // if
 
}


// создаём массив данных
$json_data = array('authorized' => $authorized, 'result' => $result);

// отсылаем его юзеру
echo json_encode($json_data);

?>