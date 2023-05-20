<?php /* Smarty version 2.6.16, created on 2019-02-04 21:31:54
         compiled from controller.tpl */ ?>

<div id="prompt_dialog" title="Подтверждение" class='hdn'>
  <p id='prompt_dialog_message'></p>
</div>

<div id="message_dialog" title="Сообщение" class='hdn'>
  <p id='message_dialog_message'></p>
</div>

<div id="data_requested_dialog" title="Обработка данных..." class='hdn'>
  <p>Пожалуйста, подождите, пока данные обрабатываются...</p>
</div>

<div id="temp_settings_dialog" title="Уставки" class='hdn'>
  <form>
  T открытия фрамуг:<br/>
  <input type='text' id='edit_t_open' maxlength='5' value='' style='width:100%;'/><br/>
  Т закрытия фрамуг:<br/>
  <input type='text' id='edit_t_close' maxlength='50' value='' style='width:100%;'/><br/>
  Время работы моторов, с:<br/>
  <input type='text' id='edit_motor_time' maxlength='100' value='' style='width:100%;'/><br/>
  </form>
</div>

<div id="sensor_name_dialog" title="Новое имя датчика" class='hdn'>
  <form>
  Имя датчика:<br/>
  <input type='text' id='edit_sensor_name' maxlength='100' value='' style='width:100%;'/><br/>
  </form>
</div>


<div id="pin_edit_dialog" title="Редактировать пин" class='hdn'>
  <form>
  Имя пина:<br/>
  <input type='text' id='edit_pin_name' maxlength='50' value='' style='width:100%;'/><br/>
  </form>
</div>



<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'controller_head.tpl', 'smarty_include_vars' => array('additional_text' => ', показания','room' => 'status')));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>

<div id='wait_block' style='padding-left:20px;'>
<img src='images/wait.gif'/ align='absmiddle'> Подождите, идёт обработка данных...
</div>

<div id='offline_block' class='hdn'>
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'controller_offline.tpl', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
</div>

<div id='online_block' class='hdn'>

  <div class='left_menu'>
      <div class='menuitem hdn' id='WELCOME_MENU' onclick="content(this);">Виджеты</div>      
      <div class='menuitem hdn' id='STATUS_MENU' onclick="content(this);">Статус</div>      
      <div class='menuitem hdn' id='TEMPERATURE_MENU' onclick="content(this);">Температура</div>
      <div class='menuitem hdn' id='HUMIDITY_MENU' onclick="content(this);">Влажность</div>
      <div class='menuitem hdn' id='LIGHT_MENU' onclick="content(this);">Освещенность</div>
      <div class='menuitem hdn' id='SOIL_MENU' onclick="content(this);">Влажность почвы</div>
      <div class='menuitem hdn' id='PH_MENU' onclick="content(this);">Показания pH</div>
      <div class='menuitem hdn' id='FLOW_MENU' onclick="content(this);">Расход воды</div>
      <div class='menuitem hdn' id='temp_motors_settings' onclick="editTempSettings();">Уставки</div>

  </div>

  <div class='page_content'>

                  <div class='content hdn' id='TEMPERATURE_MENU_CONTENT'>

                    <h1><span class='innerh'>Показания датчиков температуры</span></h1>
                    <div class='row' id='TEMPERATURE_HEADER'>
                      <div class='row_item ui-widget-header'>Название модуля</div>
                      <div class='row_item ui-widget-header'>Индекс датчика</div>
                      <div class='row_item ui-widget-header'>Температура</div>
                    </div>

                    <div id='TEMPERATURE_LIST'></div>

                  </div>


                  <div class='content hdn' id='HUMIDITY_MENU_CONTENT'>

                    <h1><span class='innerh'>Показания датчиков влажности</span></h1>
                    <div class='row' id='HUMIDITY_HEADER'>
                      <div class='row_item ui-widget-header'>Название модуля</div>
                      <div class='row_item ui-widget-header'>Индекс датчика</div>
                      <div class='row_item ui-widget-header'>Влажность</div>
                    </div>
               
                    <div id='HUMIDITY_LIST'></div>

                  </div>
                  
                  
                  <div class='content hdn' id='LIGHT_MENU_CONTENT'>

                    <h1><span class='innerh'>Показания датчиков освещенности</span></h1>
                    <div class='row' id='LUMINOSITY_HEADER'>
                      <div class='row_item ui-widget-header'>Название модуля</div>
                      <div class='row_item ui-widget-header'>Индекс датчика</div>
                      <div class='row_item ui-widget-header'>Освещенность</div>
                    </div>

                    <div id='LUMINOSITY_LIST'></div>

                  </div>

                  <div class='content hdn' id='SOIL_MENU_CONTENT'>

                    <h1><span class='innerh'>Показания датчиков влажности почвы</span></h1>
                    <div class='row' id='SOIL_HEADER'>
                      <div class='row_item ui-widget-header'>Индекс датчика</div>
                      <div class='row_item ui-widget-header'>Влажность почвы</div>
                    </div>

                    <div id='SOIL_LIST'></div>

                  </div>


                  <div class='content hdn' id='PH_MENU_CONTENT'>

                    <h1><span class='innerh'>Показания датчиков pH</span></h1>
                    <div class='row' id='PH_HEADER'>
                      <div class='row_item ui-widget-header'>Индекс датчика</div>
                      <div class='row_item ui-widget-header'>Показания pH</div>
                      <div class='row_item ui-widget-header'>Милливольт</div>
                    </div>

                    <div id='PH_LIST'></div>

                  </div>


                  <div class='content hdn' id='FLOW_MENU_CONTENT'>
                  

                    <h1><span class='innerh'>Расход воды</span></h1> 
                    
                  <div style='margin-bottom:20px;'>
                   <a href="javascript:resetFlowData();" id='reset_flow_btn'>Сбросить показания счётчиков</a>
                  </div>
                    
                    <div id='flow1_box' class='hdn'>
                      <h2><span class='innerh'>Первый расходомер</span></h2>
                      
                        <div class='half'>
                            <div class='half_box half_left'>
                                <div class='ui-widget-header ui-corner-top padding_around8px'>Мгновенный</div>
                                <div class='ui-widget-content ui-corner-bottom'> 
                                
                                  <div><img src='/images/water_meter_icon.png'/></div>
                                  <span id='flow_instant' class='bold big'>0</span><br/>литров<br/><br/>
                                </div>
                             </div>
                        </div>
                      
                        <div class='half'>
                          <div class='half_box half_right'>
                              <div class='ui-widget-header ui-corner-top padding_around8px'>Накопительный</div>
                              <div class='ui-widget-content ui-corner-bottom'>
                                <div><img src='/images/water_meter_icon.png'/></div>
                                <span id='flow_incremental' class='bold big'>0</span><br/>литров<br/><br/>
                              </div>
                          </div>
                        </div>
                      
                     </div>
                     
                    <br clear='left'/><br/> 
                    <div id='flow2_box' class='hdn'>
                      <h2><span class='innerh'>Второй расходомер</span></h2>
                      
                        <div class='half'>
                            <div class='half_box half_left'>
                                <div class='ui-widget-header ui-corner-top padding_around8px'>Мгновенный</div>
                                <div class='ui-widget-content ui-corner-bottom'> 
                                
                                  <div><img src='/images/water_meter_icon.png'/></div>
                                  <span id='flow_instant2' class='bold big'>0</span><br/>литров<br/><br/>
                                </div>
                             </div>
                        </div>
                      
                        <div class='half'>
                          <div class='half_box half_right'>
                              <div class='ui-widget-header ui-corner-top padding_around8px'>Накопительный</div>
                              <div class='ui-widget-content ui-corner-bottom'>
                                <div><img src='/images/water_meter_icon.png'/></div>
                                <span id='flow_incremental2' class='bold big'>0</span><br/>литров<br/><br/>
                              </div>
                          </div>
                        </div>
                        
                      </div>
                      
                    
                  </div>




                  <div class='content hdn' id='STATUS_MENU_CONTENT'>

                    <h1><span class='innerh'>Состояние контроллера</span></h1>
                    
                     <div class='info-info bottom-spacer'>
                      <span class='ui-icon ui-icon-info' style='float:left;margin-right:10px;'></span>
                      Обратите внимание, что обновление статуса контроллера происходит раз в 5 секунд: например, если вы нажали кнопку
                      "Закрыть все фрамуги", то актуальное состояние фрамуг может отобразиться с некоторой задержкой.
                     </div>
                                         
                    

                     <div id='mode_auto' class='hdn'><span class='auto_mode'>автоматический</span></div>
                     <div id='mode_manual' class='hdn'><span class='manual_mode'>ручной</span></div>
                      

                    <div id='mode_auto_switch' class='hdn'>Автоматический режим</div>
                    <div id='mode_manual_switch' class='hdn'>Ручной режим</div>

                    <div id='toggle_open' class='hdn'>Открыть все</div>
                    <div id='toggle_close' class='hdn'>Закрыть все</div>

                    <div id='toggle_on' class='hdn'>Включить все</div>
                    <div id='toggle_off' class='hdn'>Выключить все</div>
                    
                    <div id='window_state_on' class='hdn'>открыты <img src='images/on.png' align='absmiddle'></div>
                    <div id='window_state_off' class='hdn'>закрыты  <img src='images/off.png' align='absmiddle'></div>

                    <div id='water_state_on' class='hdn'>включён <img src='images/on.png' align='absmiddle'></div>
                    <div id='water_state_off' class='hdn'>выключен <img src='images/off.png' align='absmiddle'></div>

                    <div id='ph_state_on' class='hdn'><span class='auto_mode'>включён</span> <img src='images/on.png' align='absmiddle'></div>
                    <div id='ph_state_off' class='hdn'><span class='manual_mode'>выключен</span> <img src='images/off.png' align='absmiddle'></div>

                    <div id='light_state_on' class='hdn'>включёна <img src='images/on.png' align='absmiddle'></div>
                    <div id='light_state_off' class='hdn'>выключена <img src='images/off.png' align='absmiddle'></div>
                    
                    
                    <div id='windows_controller_status' class='hdn'>
                    
                      <div class='ui-widget-header ui-corner-top padding_around8px'>Окна <span class='bold right_float' id='window_state'></span></div>   
                      <div class='ui-widget-content  padding_around8px'>
                      
                              <div class='padding_around8px'>
                                Режим: <span class='bold' id='window_mode'></span>
                              </div> 
                              
                              <div id='windowsChannelsState' class='hdn bottom-spacer'>
                              </div>                                                   
                      
                         

                      </div>
                      
                      <div class='ui-widget-footer padding_around8px ui-corner-bottom'>
                              <button id='toggler_windows' onclick='controller.toggleWindows();updateWindowsState();'></button>
                              <button id='toggler_windows_mode' onclick='controller.toggleWindowsMode();updateWindowsState();'></button>
                      </div>
                       
                    <br/><br/>
                     </div>
                     
                    <div id='water_controller_status' class='hdn'>
                                       
                    <div class='ui-widget-header ui-corner-top padding_around8px'>Полив <span class='bold right_float' id='water_state'></span></div> 
                    <div class='ui-widget-content padding_around8px'>
                    
                        <div class='padding_around8px'>
                                Режим: <span class='bold' id='water_mode'></span>
                         </div>
                             
            
                         <div id='waterChannelsState' class='hdn bottom-spacer'>
                         
                         </div>
                         
                         <div>
                          <button id="skip_watering_btn" onclick="skipTodayWatering();">Пропустить полив сегодня</button>
                         </div>
                        
                    
                               
                        
                    </div>

                    <div class='ui-widget-footer padding_around8px ui-corner-bottom'>
                              <button id='toggler_water' onclick='controller.toggleWater();updateWaterState();'></button>
                              <button id='toggler_water_mode' onclick='controller.toggleWaterMode();updateWaterState();'></button>
                    </div>
                    
                    <br/><br/>
                    </div>
                    
                    <div id='light_controller_status' class='hdn'>

                    <div class='ui-widget-header ui-corner-top padding_around8px'>Досветка <span class='bold right_float' id='light_state'></span></div>
                    <div class='ui-widget-content upadding_around8px'>
                    
                        <div class='padding_around8px'>
                            Режим: <span class='bold' id='light_mode'></span>
                        </div>
                        
                    </div>
                    
                    <div class='ui-widget-footer padding_around8px ui-corner-bottom'>
                        <button id='toggler_light' onclick='controller.toggleLight();updateLightState();'></button>
                        <button id='toggler_light_mode' onclick='controller.toggleLightMode();updateLightState();'></button>
                    </div>
                    
                    <br/><br/>
                    
                    </div>
  
                  
                  <div id='ph_controller_status' class='hdn'>
                  
                    <div class='ui-widget-header ui-corner-top padding_around8px'>PH</div>
                    <div class='ui-widget-content ui-corner-bottom padding_around8px'>
                    
                         <div class='padding_around8px'>
                            Насос подачи воды: <span class='bold' id='ph_flow_add'></span><br/>
                            Насос перемешивания: <span class='bold' id='ph_mix_pump'></span><br/>
                            Насос увеличения pH: <span class='bold' id='ph_plus_pump'></span><br/>
                            Насос уменьшения pH: <span class='bold' id='ph_minus_pump'></span><br/>
                        </div>                     
                    
                    </div>
                  
                  <br/><br/>
                  </div>
                  
                  
                  <div id='pins_status' class='hdn'>
                  
                    <div class='ui-widget-header ui-corner-top padding_around8px'>Пины</div>
                    <div class='ui-widget-content ui-corner-bottom padding_around8px'>
                    
                         <div class='padding_around8px' id="pins_list">
                            
                        </div>                     
                    
                    </div>
                  
                  <br/><br/>
                  </div>
                  
                  <div id='scenes_status' class='hdn'>
                  
                    <div class='ui-widget-header ui-corner-top padding_around8px'>Сценарии</div>
                    <div class='ui-widget-content ui-corner-bottom padding_around8px'>
                    
                        <div class='button_menu_spacer'>
                          <select id='scene_list' size=8>
                          </select>
                        </div>                   
                        
                      <div style='margin-top:10px;'>
                          <button id='run_scene' onclick='runScene();'>Запустить выбранный сценарий</button>                        
                          <button id='stop_scene' onclick='stopScene();'>Остановить выбранный сценарий</button>                        
                      </div>                             
                    
                    </div>
                  
                  <br/><br/>
                  </div>                  
                  
              </div>

                  <div class='content hdn' id='WELCOME_MENU_CONTENT'>

                    <h1><span class='innerh'>Виджеты</span></h1>
                    
                    <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'controller_widgets.tpl', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>

                    
                  </div>

  </div>

</div>


<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "controller_helpers.tpl", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>


<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "commands_help.tpl", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>