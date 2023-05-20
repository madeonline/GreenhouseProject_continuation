<?php
function createIfNotExists($tableName,$createSQL)
{
  global $dbengine;
  $sql = "SELECT COUNT(*) AS tot_cnt FROM sqlite_master WHERE type='table' AND name='$tableName';";
      $res = $dbengine->query($sql);
      if($res !== FALSE)
      {
        if($array = $res->fetchArray())
        {
          $exists = intval($array['tot_cnt']) > 0;
          if(!$exists)
          {
            // создаём таблицу
            $dbengine->beginTransaction();
            $dbengine->query($createSQL);
            $dbengine->commitTransaction();
          }
        }
      }  
}
?>