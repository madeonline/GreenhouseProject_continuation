<?php /* Smarty version 2.6.16, created on 2019-02-04 21:31:39
         compiled from controller_info.tpl */ ?>

<div  style='float:left;' id='controller<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
'>

<fieldset class='controller_info_box ui-corner-all' style='position:relative;'>
<legend class='ui-widget-header ui-corner-all'><span id='controller_name<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
'><?php echo $this->_tpl_vars['controller']['controller_name']; ?>
</span></legend>

<span onclick='deleteController(<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
)' class='ui-icon ui-icon-close' style='cursor:pointer;position:absolute;top:15px;right:5px;' title='Удалить контроллер'></span>


ID контроллера: <span id='controller_id<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
'><?php echo $this->_tpl_vars['controller']['controller_id']; ?>
</span><br/>
Адрес контроллера: <span id='controller_address<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
'><?php echo $this->_tpl_vars['controller']['controller_address']; ?>
</span><br/>
Онлайн: <b><span id='controller_status<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
'><?php if ($this->_tpl_vars['controller']['is_online'] == 1): ?><span class='auto_mode'>да</span><?php else: ?><span class='manual_mode'>нет</span><?php endif; ?></span></b><br/>

<div style='margin-top:8px;'>

<table width='100%' border='0' cellspacing='0' cellpadding='2'>
<tr>
<td width='50%'>
    <a href="javascript:editController(<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
);" id='controller_edit_link<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
' class='controller-edit-btn'>Редактировать</a>
</td>
<td width='50%'>

    <a href="controller.php?id=<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
" id='controller_view_link<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
' class='controller-data-btn'>Показания</a>
</td>

</tr>
<tr>
<td width='50%'>
    <a href="controller_settings.php?id=<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
" id='controller_settings_link<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
' class='controller-settings-btn'>Настройки</a>
</td>
<td width='50%'>
    <a href="controller_charts.php?id=<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
" id='controller_charts_link<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
' class='controller-charts-btn'>Графики</a>
</td>
</tr>
</table>
</div>

</fieldset>
</div>