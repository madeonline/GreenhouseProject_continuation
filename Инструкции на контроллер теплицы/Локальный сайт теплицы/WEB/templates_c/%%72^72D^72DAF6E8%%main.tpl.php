<?php /* Smarty version 2.6.16, created on 2019-03-14 13:45:44
         compiled from main.tpl */ ?>

<script type="text/javascript" src="js/edit_controller.js"></script>

<div id='no_controllers_hint' class='hdn'>
  <div class="controller_list_hint ui-state-error ui-corner-all error">
    <img src="/images/logo48.png" class="logo">Нет ни одного контроллера. Для добавления контроллера нажмите кнопку ниже.
  </div>
</div>

<div id="controller_dialog" title="Данные контроллера" class='hdn'>
  <form>
    Имя контроллера:<br/>
  <input type='text' id='controller_name' maxlength='50' value='' style='width:100%;'/><br/>
  Адрес контроллера:<br/>
  <input type='text' id='controller_address' maxlength='100' value='' style='width:100%;'/><br/>
  </form>
</div>

<div id='message_box_dialog' class='hdn'>
  <div id='message_box_text'></div>
</div>

<div id='controller_online' class='hdn'><span class='auto_mode'>да</span></div>
<div id='controller_offline' class='hdn'><span class='manual_mode'>нет</span></div>

<div id="controller_in_process_dialog" title="Идёт обработка данных..." class='hdn'>
  <p>Пожалуйста, подождите, пока данные обрабатываются...</p>
</div>

<script type='text/javascript'>
var controllers = new Array();

<?php echo '
function setupController(c) {
    c.OnStatus = function(controller) {
      var stat = controller.IsOnline() ? $(\'#controller_online\').html() :  $(\'#controller_offline\').html();
      $("#controller_status" + controller.getId()).html(stat);
    };
} 

function addController(id,name,address) {
  var newC = new Controller(id,name,address);
  controllers.push(newC);
  return newC;
}
  
 $(document).ready(function(){
 
  $( "#add_new_controller_btn" ).button({
      icons: {
        primary: "ui-icon-plus"
      }
    });
    
  
 });
 
 $(document).ready(function(){
  setupControllerButtons();
  addControllerListHint();
 });
  
'; ?>


<?php $_from = $this->_tpl_vars['controllers']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['k'] => $this->_tpl_vars['controller']):
?>
addController(<?php echo $this->_tpl_vars['controller']['controller_id']; ?>
,'<?php echo $this->_tpl_vars['controller']['controller_name']; ?>
','<?php echo $this->_tpl_vars['controller']['controller_address']; ?>
');
<?php endforeach; endif; unset($_from); ?>

<?php echo '
  for(var i=0;i<controllers.length;i++)
  {
    var c = controllers[i];
    setupController(c);

  }
'; ?>


</script>

<h1><span class='innerh'>Список контроллеров</span></h1>
<div id='controllers_list'>
<?php $_from = $this->_tpl_vars['controllers']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['k'] => $this->_tpl_vars['controller']):
 $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'controller_info.tpl', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
  endforeach; endif; unset($_from); ?>
</div>

<div style='clear:both;padding-top:20px;'>
  <a href='#' onclick="addNewController();return false;" id='add_new_controller_btn'>Добавить новый контроллер</a>
</div>