<?php /* Smarty version 2.6.16, created on 2019-02-04 21:32:14
         compiled from timer.tpl */ ?>

<div class='ui-widget-header ui-corner-top padding_around8px'>Таймер №<?php echo $this->_tpl_vars['num']; ?>
</div>
<div class='ui-widget-content ui-corner-bottom' style='text-align:left;padding:8px;'> 

  <table border='0' width='100%'>
  <tr><td width='50%' valign='top'>
  <div class='button_menu_spacer'>
  Пин: <br/>
  <input type='text' id='timerPin<?php echo $this->_tpl_vars['num']; ?>
' maxlength='2' style='width:50px;'/>
  </div>
  <div class='button_menu_spacer'>
  Включён (мин, сек):<br/>
  <input type='text' id='timerOnMin<?php echo $this->_tpl_vars['num']; ?>
' maxlength='4' style='width:50px;'/> : <input type='text' id='timerOnSec<?php echo $this->_tpl_vars['num']; ?>
' maxlength='2' style='width:50px;'/>
  </div>
  Выключен (мин, сек):<br/>
  <input type='text' id='timerOffMin<?php echo $this->_tpl_vars['num']; ?>
' maxlength='4' style='width:50px;'/> : <input type='text' id='timerOffSec<?php echo $this->_tpl_vars['num']; ?>
' maxlength='2' style='width:50px;'/>

  </td>
  <td valign='top'>
  
  <div id='timerDayMask<?php echo $this->_tpl_vars['num']; ?>
'>
    <input type='checkbox' value='0'>Понедельник<br/>
    <input type='checkbox' value='1'>Вторник<br/>
    <input type='checkbox' value='2'>Среда<br/>
    <input type='checkbox' value='3'>Четверг<br/>
    <input type='checkbox' value='4'>Пятница<br/>
    <input type='checkbox' value='5'>Суббота<br/>
    <input type='checkbox' value='6'>Воскресенье<br/>
  </div>
  </td>
  </tr>
  </table>
  
  <input type='checkbox' id='timerEnabled<?php echo $this->_tpl_vars['num']; ?>
'/><label for='timerEnabled<?php echo $this->_tpl_vars['num']; ?>
'>Таймер активен?</label>
  
</div>