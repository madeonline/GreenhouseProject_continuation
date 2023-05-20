<?php
// эта строчка обязательна к подключению всеми скриптами
require_once($_SERVER['DOCUMENT_ROOT'] . "/prerequisites.php");

$result = false;

// создаём массив данных
$json_data = array('authorized' => $authorized, 'result' => $result);

if($authorized)
{
  $posted = intval(@$_GET['posted']);
  if($posted == 1)
  {
   $controller_name = $dbengine->escapeString(mb_substr(strip_tags(@$_GET['controller_name']),0,50));
   $c_addr = $dbengine->escapeString(mb_substr(strip_tags(@$_GET['controller_address']),0,100));
  
    if($controller_name != '' && $c_addr != '')
    {
      // обновляем базу
      $dbengine->beginTransaction();
      //$dbengine->exec("UPDATE controllers SET controller_name='$controller_name', controller_address='$c_addr' WHERE controller_id=$controller_id ;");
      $sql = "SELECT MAX(controller_id)+1 AS new_id FROM controllers;";
      $res = $dbengine->query($sql);
      if($res !== FALSE) 
      {
        $row = $res->fetchArray();
        if($row !== FALSE)
          $new_id = intval($row['new_id']);
         else
          $new_id = 1; // нет записей?
      } 
      else 
      {
        $new_id = 1; // нет записей?
      }
      
      if($new_id < 1)
        $new_id = 1;
        
        $sql = "INSERT INTO controllers(controller_id,controller_name,is_online,controller_address) VALUES($new_id,'$controller_name',0,'$c_addr');";
        
        $result = $dbengine->exec($sql);
        $json_data['result'] = $result;
        
        if($result) {
          // контроллер добавлен, можно возвращать данные
          $json_data['id'] = $new_id;
          $json_data['name'] = $controller_name;
          $json_data['address'] = $c_addr;
          
          $tplEngine->assign('controller',[
          
            'controller_id' => $new_id,
            'controller_name' => $controller_name,
            'is_online' => 0,
            'controller_address' => $c_addr,
          
          ]);
          
          $json_data['box'] = $tplEngine->fetch('controller_info.tpl');
        }
        
      
      
      $dbengine->commitTransaction();
    }
    
  
  
      
  } // if
 
}

// отсылаем его юзеру
echo json_encode($json_data);

?>