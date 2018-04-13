using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace CoreConfig
{
    class NumericUpDownColumn : DataGridViewColumn
    {
        public NumericUpDownColumn()
            : base(new NumericUpDownCell())

        {

        }

        public override DataGridViewCell CellTemplate
        {
            get
            {
                return base.CellTemplate;
            }
            set
            {
                // Ensure that the cell used for the template is a NumericUpDownCell.
                if (value != null &&
                    !value.GetType().IsAssignableFrom(typeof(NumericUpDownCell)))
                {
                    throw new InvalidCastException("Must be a NumericUpDownCell");
                }
                base.CellTemplate = value;
            }
        }
    }

    public class NumericUpDownCell : DataGridViewTextBoxCell
    {
        public NumericUpDownCell()
            : base()
        {
        }
        /*
        public override void PositionEditingControl(bool setLocation, bool setSize, Rectangle cellBounds, Rectangle cellClip, DataGridViewCellStyle cellStyle, bool singleVerticalBorderAdded, bool singleHorizontalBorderAdded, bool isFirstDisplayedColumn, bool isFirstDisplayedRow)
        {
            cellClip.Height = 100;
            cellBounds.Height = 100;
            Control ec = DataGridView.EditingControl;
            base.PositionEditingControl(setLocation, setSize, cellBounds, cellClip, cellStyle, singleVerticalBorderAdded, singleHorizontalBorderAdded, isFirstDisplayedColumn, isFirstDisplayedRow);
            //DataGridView.Controls.Remove(ec);
            //DataGridView.Parent.Controls.Add(ec);
            ec.Parent = DataGridView.Parent;
            ec.Left = DataGridView.Left + cellBounds.Left;
            ec.Top = DataGridView.Top + cellBounds.Top;
            ec.Height = 60;
            ec.BringToFront();

            ec.LostFocus += new EventHandler(ec_LostFocus);

        }

        void ec_LostFocus(object sender, EventArgs e)
        {
            DataGridView.EndEdit();

        }*/



        protected override bool SetValue(int rowIndex, object value)
        {
            formattedValue = FormatValue(Convert.ToInt32(value));
            return base.SetValue(rowIndex, formattedValue.Length > 0 ? formattedValue : null);
        }

        private string formattedValue = "";

        public override void InitializeEditingControl(int rowIndex, object
            initialFormattedValue, DataGridViewCellStyle dataGridViewCellStyle)
        {
            // Set the value of the editing control to the current cell value.
            base.InitializeEditingControl(rowIndex, initialFormattedValue, dataGridViewCellStyle);

            CheckListBoxEditingControl ctl = DataGridView.EditingControl as CheckListBoxEditingControl;

            // Use the default row value when Value property is null.
            WateringChannelSettings s = (WateringChannelSettings)this.DataGridView.Rows[rowIndex].Tag;
            ctl.Tag = s;

            if (this.Value == null)
            {
                ctl.Value = "";
            }
            else
            {
                ctl.Value = FormatValue(s.WateringDays);
            }

        }

        private string[] wDaysNames = { "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс" };
        public string FormatValue(int weekDays)
        {
            String s = "";
            for (byte i = 0; i < 7; i++)
            {
                int check = 1 << i;
                if ((weekDays & check) == check)
                {
                    if (s.Length > 0)
                        s += ",";

                    string wday = AppSettings.Instance.Localizer.GetValue("WeekdaysShort", i.ToString());
                    if (wday.Length < 1)
                        wday = wDaysNames[i];

                    s += wday;
                }

            }
            return s;
        }



        public override void DetachEditingControl()
        {
            DataGridView dataGridView = this.DataGridView;

            if (dataGridView == null || dataGridView.EditingControl == null)
            {
                throw new InvalidOperationException("Cell is detached or its grid has no editing control.");
            }

            CheckListBoxEditingControl ctl = DataGridView.EditingControl as CheckListBoxEditingControl;
            if (ctl != null)
            {
                WateringChannelSettings s = (WateringChannelSettings)ctl.Tag;
                this.Value = s.WateringDays.ToString();
            }


            base.DetachEditingControl();
        }

        public override Type EditType
        {
            get
            {
                // Return the type of the editing control that CalendarCell uses.
                return typeof(CheckListBoxEditingControl);
            }
        }

        public override Type ValueType
        {
            get
            {
                // Return the type of the value that cell contains.

                return typeof(string);
            }
        }
        public override object DefaultNewRowValue
        {
            get
            {
                // Use the current date and time as the default value.
                return null;
            }
        }

    }


}

class NumericEditingControl : NumericUpDown, IDataGridViewEditingControl
{

    DataGridView dataGridView;
    private bool valueChanged = false;
    int rowIndex;

    int signalNumber = 0; // какой сигнал выставляем


    protected override void OnItemCheck(ItemCheckEventArgs ice)
    {
        WateringChannelSettings s = (WateringChannelSettings)this.Tag;

        bool isChecked = ice.NewValue == CheckState.Checked;
        int idx = ice.Index;

        int mask = 1 << idx;
        if (isChecked)
            s.WateringDays = s.WateringDays | mask;
        else
            s.WateringDays = s.WateringDays & ~mask;

        base.OnItemCheck(ice);

        valueChanged = true;
        //this.OnValueMemberChanged(new EventArgs());
        dataGridView.NotifyCurrentCellDirty(true);
    }


    private void SetValue(string val)
    {
        // тут нам передали значение на расшифровку
        valueChanged = true;
        WateringChannelSettings s = (WateringChannelSettings)this.Tag;
        weekDays = s.WateringDays;

        // устанавливаем индексы выделенных дней
        for (byte i = 0; i < 7; i++)
        {
            int check = 1 << i;
            if ((weekDays & check) == check)
            {
                this.SetItemChecked(i, true);
            }
            else
                this.SetItemChecked(i, false);
        }
    }

    public string Value
    {
        get
        {
            WateringChannelSettings s = (WateringChannelSettings)this.Tag;
            return s.WateringDays.ToString();
        }
        set { SetValue(value); }
    }

    public CheckListBoxEditingControl()
    {
        this.Items.Add(AppSettings.Instance.Localizer["WeekDay1"]);
        this.Items.Add(AppSettings.Instance.Localizer["WeekDay2"]);
        this.Items.Add(AppSettings.Instance.Localizer["WeekDay3"]);
        this.Items.Add(AppSettings.Instance.Localizer["WeekDay4"]);
        this.Items.Add(AppSettings.Instance.Localizer["WeekDay5"]);
        this.Items.Add(AppSettings.Instance.Localizer["WeekDay6"]);
        this.Items.Add(AppSettings.Instance.Localizer["WeekDay7"]);

        this.CheckOnClick = true;
    }


    // Implements the IDataGridViewEditingControl.EditingControlFormattedValue 
    // property.
    public object EditingControlFormattedValue
    {
        get
        {
            return this.Value;
        }
        set
        {
            if (value is string)
            {
                try
                {
                    // This will throw an exception of the string is 
                    // null, empty, or not in the format of a date.
                    this.SetValue((String)value);
                }
                catch
                {
                    // In the case of an exception, just use the 
                    // default value so we're not left with a null
                    // value.
                    this.Value = "";
                }
            }
        }
    }

    // Implements the 
    // IDataGridViewEditingControl.GetEditingControlFormattedValue method.
    public object GetEditingControlFormattedValue(
        DataGridViewDataErrorContexts context)
    {
        return this.EditingControlFormattedValue;
    }

    // Implements the 
    // IDataGridViewEditingControl.ApplyCellStyleToEditingControl method.
    public void ApplyCellStyleToEditingControl(
        DataGridViewCellStyle dataGridViewCellStyle)
    {
        this.Font = dataGridViewCellStyle.Font;
        this.ForeColor = dataGridViewCellStyle.ForeColor;
        this.BackColor = dataGridViewCellStyle.BackColor;
    }

    // Implements the IDataGridViewEditingControl.EditingControlRowIndex 
    // property.
    public int EditingControlRowIndex
    {
        get
        {
            return rowIndex;
        }
        set
        {
            rowIndex = value;
        }
    }

    // Implements the IDataGridViewEditingControl.EditingControlWantsInputKey 
    // method.
    public bool EditingControlWantsInputKey(
        Keys key, bool dataGridViewWantsInputKey)
    {
        // Let the DateTimePicker handle the keys listed.
        switch (key & Keys.KeyCode)
        {
            case Keys.Left:
            case Keys.Up:
            case Keys.Down:
            case Keys.Right:
            case Keys.Home:
            case Keys.End:
            case Keys.PageDown:
            case Keys.PageUp:
                return true;
            default:
                return !dataGridViewWantsInputKey;
        }
    }

    // Implements the IDataGridViewEditingControl.PrepareEditingControlForEdit 
    // method.
    public void PrepareEditingControlForEdit(bool selectAll)
    {
        // No preparation needs to be done.
    }

    // Implements the IDataGridViewEditingControl
    // .RepositionEditingControlOnValueChange property.
    public bool RepositionEditingControlOnValueChange
    {
        get
        {
            return false;
        }
    }

    // Implements the IDataGridViewEditingControl
    // .EditingControlDataGridView property.
    public DataGridView EditingControlDataGridView
    {
        get
        {
            return dataGridView;
        }
        set
        {
            dataGridView = value;
        }
    }

    // Implements the IDataGridViewEditingControl
    // .EditingControlValueChanged property.
    public bool EditingControlValueChanged
    {
        get
        {
            return valueChanged;
        }
        set
        {
            valueChanged = value;
        }
    }

    // Implements the IDataGridViewEditingControl
    // .EditingPanelCursor property.
    public Cursor EditingPanelCursor
    {
        get
        {
            return base.Cursor;
        }
    }



    protected override void OnValueMemberChanged(EventArgs eventargs)
    {
        // Notify the DataGridView that the contents of the cell
        // have changed.
        valueChanged = true;
        this.EditingControlDataGridView.NotifyCurrentCellDirty(true);
        base.OnValueMemberChanged(eventargs);
    }

}
