using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Globalization;
using System.Reflection;
using System.Drawing.Design;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace UROVConfig
{
    public enum TreeNodeType
    {
        MainSettingsNode,
        SDSettingsNode,
    }

    public enum AnswerBehaviour
    {
        Normal,
        SDCommandLS,
        SDCommandFILE,
    }

    public enum SDNodeTags
    {
        TagDummyNode = 100,
        TagFolderUninitedNode,
        TagFolderNode,
        TagFileNode
    }

    public enum FileDownloadFlags
    {
        View,
        DownloadEthalon
    }

    public class SDNodeTagHelper
    {
        public SDNodeTags Tag = SDNodeTags.TagFolderUninitedNode;
        public string FileName = "";
        public bool IsDirectory = false;
        public SDNodeTagHelper(SDNodeTags tag, string fileName, bool isDir)
        {
            Tag = tag;
            FileName = fileName;
            IsDirectory = isDir;
        }
    }

    public class MyEnumConverter : EnumConverter
    {
        private Type type;

        public MyEnumConverter(Type type)
            : base(type)
        {
            this.type = type;
        }


        public override object ConvertTo(ITypeDescriptorContext context,
            CultureInfo culture, object value, Type destType)
        {
            try
            {
                FieldInfo fi = type.GetField(Enum.GetName(type, value));
                DescriptionAttribute descAttr =
                  (DescriptionAttribute)Attribute.GetCustomAttribute(
                    fi, typeof(DescriptionAttribute));

                if (descAttr != null)
                    return descAttr.Description;
                else
                    return value.ToString();
            }
            catch
            {
                return Enum.GetValues(type).GetValue(0);
            }
        }

        public override object ConvertFrom(ITypeDescriptorContext context,
            CultureInfo culture, object value)
        {
            foreach (FieldInfo fi in type.GetFields())
            {
                DescriptionAttribute descAttr =
                  (DescriptionAttribute)Attribute.GetCustomAttribute(
                    fi, typeof(DescriptionAttribute));

                if ((descAttr != null) && ((string)value == descAttr.Description))
                    return Enum.Parse(type, fi.Name);
            }
            return Enum.Parse(type, (string)value);
        }
    }

    public class EnumHelpers
    {
        public static string GetEnumDescription(Enum value)
        {
            FieldInfo fi = value.GetType().GetField(value.ToString());

            DescriptionAttribute[] attributes =
                (DescriptionAttribute[])fi.GetCustomAttributes(
                typeof(DescriptionAttribute),
                false);

            if (attributes != null &&
                attributes.Length > 0)
                return attributes[0].Description;
            else
                return value.ToString();
        }

        public static T GetValueFromDescription<T>(string description)
        {
            var type = typeof(T);
            if (!type.IsEnum) throw new InvalidOperationException();
            foreach (var field in type.GetFields())
            {
                var attribute = Attribute.GetCustomAttribute(field,
                    typeof(DescriptionAttribute)) as DescriptionAttribute;
                if (attribute != null)
                {
                    if (attribute.Description == description)
                        return (T)field.GetValue(null);
                }
                else
                {
                    if (field.Name == description)
                        return (T)field.GetValue(null);
                }
            }
            throw new ArgumentException("Not found.", "description");
            // or return default(T);
        }
    }

    class YesNoConverter : BooleanConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
        {
            if (value is bool && destinationType == typeof(string))
            {
                return values[(bool)value ? 1 : 0];
            }
            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value)
        {
            string txt = value as string;
            if (values[0] == txt) return false;
            if (values[1] == txt) return true;
            return base.ConvertFrom(context, culture, value);
        }

        private string[] values = new string[] { "Нет", "Да" };
    }

    /// <summary>
    /// Range modification for direct edit override
    /// </summary>
    public class NumericUpDownTypeConverter : TypeConverter
    {
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            // Attempt to do them all
            return true;
        }


        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            try
            {
                string Value;
                if (!(value is string))
                {
                    Value = Convert.ChangeType(value, context.PropertyDescriptor.PropertyType).ToString();
                }
                else
                    Value = value as string;
                decimal decVal;
                if (!decimal.TryParse(Value, out decVal))
                    decVal = decimal.One;
                MinMaxAttribute attr = (MinMaxAttribute)context.PropertyDescriptor.Attributes[typeof(MinMaxAttribute)];
                if (attr != null)
                {
                    decVal = attr.PutInRange(decVal);
                }
                return Convert.ChangeType(decVal, context.PropertyDescriptor.PropertyType);
            }
            catch
            {
                return base.ConvertFrom(context, culture, value);
            }
        }

        public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destinationType)
        {
            try
            {
                return destinationType == typeof(string)
                   ? Convert.ChangeType(value, context.PropertyDescriptor.PropertyType).ToString()
                   : Convert.ChangeType(value, destinationType);
            }
            catch { }
            return base.ConvertTo(context, culture, value, destinationType);
        }
    }

    // ReSharper disable MemberCanBePrivate.Global
    /// <summary>
    /// Attribute to allow ranges to be added to the numeric updowner
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false)]
    public class MinMaxAttribute : Attribute
    {
        public decimal Min { get; private set; }
        public decimal Max { get; private set; }
        public decimal Increment { get; private set; }
        public int DecimalPlaces { get; private set; }

        /// <summary>
        /// Use to make a simple UInt16 max. Starts at 0, increment = 1
        /// </summary>
        /// <param name="max"></param>
        public MinMaxAttribute(UInt16 max)
           : this((decimal)UInt16.MinValue, max)
        {
        }

        /// <summary>
        /// Use to make a simple integer (or default conversion) based range.
        /// default inclrement is 1
        /// </summary>
        /// <param name="min"></param>
        /// <param name="max"></param>
        /// <param name="increment"></param>
        public MinMaxAttribute(int min, int max, int increment = 1)
           : this((decimal)min, max, increment)
        {
        }

        /// <summary>
        /// Set the Min, Max, increment, and decimal places to be used.
        /// </summary>
        /// <param name="min"></param>
        /// <param name="max"></param>
        /// <param name="increment"></param>
        /// <param name="decimalPlaces"></param>
        public MinMaxAttribute(decimal min, decimal max, decimal increment = decimal.One, int decimalPlaces = 0)
        {
            Min = min;
            Max = max;
            Increment = increment;
            DecimalPlaces = decimalPlaces;
        }

        /// <summary>
        /// Validation function to check if the value is withtin the range (inclusive)
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public bool IsInRange(object value)
        {
            decimal checkedValue = (decimal)Convert.ChangeType(value, typeof(decimal));
            return ((checkedValue <= Max)
               && (checkedValue >= Min)
               );
        }

        /// <summary>
        /// Takes the value and adjusts if it is out of bounds.
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public decimal PutInRange(object value)
        {
            decimal checkedValue = (decimal)Convert.ChangeType(value, typeof(decimal));
            if (checkedValue > Max)
                checkedValue = Max;
            else if (checkedValue < Min)
                checkedValue = Min;
            return checkedValue;
        }
    }
    // ReSharper restore MemberCanBePrivate.Global


    public class NumericUpDownTypeEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
        {
            if (context == null || context.Instance == null)
                return base.GetEditStyle(context);
            return context.PropertyDescriptor.IsReadOnly ? UITypeEditorEditStyle.None : UITypeEditorEditStyle.DropDown;
        }

        public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            try
            {
                if (context == null || context.Instance == null || provider == null)
                    return value;

                //use IWindowsFormsEditorService object to display a control in the dropdown area  
                IWindowsFormsEditorService frmsvr = (IWindowsFormsEditorService)provider.GetService(typeof(IWindowsFormsEditorService));
                if (frmsvr == null)
                    return value;

                MinMaxAttribute attr = (MinMaxAttribute)context.PropertyDescriptor.Attributes[typeof(MinMaxAttribute)];
                if (attr != null)
                {
                    NumericUpDown nmr = new NumericUpDown
                    {
                        Size = new Size(60, 120),
                        Minimum = attr.Min,
                        Maximum = attr.Max,
                        Increment = attr.Increment,
                        DecimalPlaces = attr.DecimalPlaces,
                        Value = attr.PutInRange(value)
                    };
                    frmsvr.DropDownControl(nmr);
                    context.OnComponentChanged();
                    return Convert.ChangeType(nmr.Value, context.PropertyDescriptor.PropertyType);
                }
            }
            catch { }
            return value;
        }
    }






    public class CsvConverter : TypeConverter
    {
        // Overrides the ConvertTo method of TypeConverter.
        public override object ConvertTo(ITypeDescriptorContext context,
           CultureInfo culture, object value, Type destinationType)
        {
            List<String> v = value as List<String>;
            if (destinationType == typeof(string))
            {
                return String.Join(",", v.ToArray());
            }
            return base.ConvertTo(context, culture, value, destinationType);
        }
    }


 /* 

    public class ConfigGeneralSettings
    {

        
        [Browsable(true)]
        [Category("1. Моторесурс текущий")]
        [Description("Текущий моторесурс канала №1.")]
        [DisplayName("Канал №1")]
        [TypeConverter(typeof(NumericUpDownTypeConverter))]
        [Editor(typeof(NumericUpDownTypeEditor), typeof(UITypeEditor)), MinMaxAttribute(0, 100000000)]
        public int MotoresourceCurrent1 { get { return Config.Instance.MotoresourceCurrent1; } set { Config.Instance.MotoresourceCurrent1 = value; } }
        public bool ShouldSerializeMotoresourceCurrent1() { return false; }

        [Browsable(true)]
        [Category("2. Информация")]
        [Description("Количество свободной памяти, байт.")]
        [DisplayName("Память")]
        [ReadOnly(true)]
        public int FreeRAM { get { return Config.Instance.FreeRAM; }  }
        public bool ShouldSerializeFreeRAM() { return false; }


        [Browsable(true)]
        [Category("2. Информация")]
        [Description("Версия ядра, используемого прошивкой.")]
        [DisplayName("Версия ядра")]
        [ReadOnly(true)]
        public string CoreVersion { get { return Config.Instance.CoreVersion; } }
        public bool ShouldSerializeCoreVersion() { return false; }

        [Browsable(true)]
        [Category("2. Информация")]
        [Description("Время, установленное на контроллере.")]
        [DisplayName("Время")]
        [ReadOnly(true)]
        public string ControllerDateTime
        {
            get
            {
                if (Config.Instance.ControllerDateTime.Length < 1)
                    return "-";

                return Config.Instance.ControllerDateTime;
            }

        }
        public bool ShouldSerializeControllerDateTime() { return false; }



    }
*/
    public class FeaturesSettings
    {
        private bool sdAvailable = true;
        public bool SDAvailable { get { return sdAvailable; } }


        public void Clear()
        {
            sdAvailable = true;
        }

    }

    public class Config
    {


        private static object lockFlag = new object();
        private static Config instance;

        public static Config Instance
        {
            get
            {
                lock (lockFlag)
                {
                    if (instance == null)
                    {
                       
                        instance = new Config();                     

                    } // if instance == null
                } // lock
                return instance;
            } // get

        }

        private Config()
        {
        }

        public void Clear()
        {
            motoresourceCurrent1 = 0;
            motoresourceCurrent2 = 0;
            motoresourceCurrent3 = 0;

            motoresourceMax1 = 0;
            motoresourceMax2 = 0;
            motoresourceMax3 = 0;

            pulses1 = 0;
            pulses2 = 0;
            pulses3 = 0;

            delta1 = 0;
            delta2 = 0;
            delta3 = 0;
        }

        private int motoresourceCurrent1 = 0;
        private int motoresourceCurrent2 = 0;
        private int motoresourceCurrent3 = 0;

        public int MotoresourceCurrent1 { get { return motoresourceCurrent1; } set { motoresourceCurrent1 = value; } }
        public int MotoresourceCurrent2 { get { return motoresourceCurrent2; } set { motoresourceCurrent2 = value; } }
        public int MotoresourceCurrent3 { get { return motoresourceCurrent3; } set { motoresourceCurrent3 = value; } }

        private int motoresourceMax1 = 0;
        private int motoresourceMax2 = 0;
        private int motoresourceMax3 = 0;

        public int MotoresourceMax1 { get { return motoresourceMax1; } set { motoresourceMax1 = value; } }
        public int MotoresourceMax2 { get { return motoresourceMax2; } set { motoresourceMax2 = value; } }
        public int MotoresourceMax3 { get { return motoresourceMax3; } set { motoresourceMax3 = value; } }

        private int pulses1 = 0;
        private int pulses2 = 0;
        private int pulses3 = 0;

        public int Pulses1 { get { return pulses1; } set { pulses1 = value; } }
        public int Pulses2 { get { return pulses2; } set { pulses2 = value; } }
        public int Pulses3 { get { return pulses3; } set { pulses3 = value; } }

        private int delta1 = 0;
        private int delta2 = 0;
        private int delta3 = 0;

        public int Delta1 { get { return delta1; } set { delta1 = value; } }
        public int Delta2 { get { return delta2; } set { delta2 = value; } }
        public int Delta3 { get { return delta3; } set { delta3 = value; } }


    }



}
