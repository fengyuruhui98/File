using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.ComponentModel;
using System.Xml.Linq;

namespace HHJT.AFC.Base.API
{
    public class XMLManipulater
    {
        public static XmlNode AddElement(string tagName, string textContent, XmlNode parent)
        {
            XmlNode node = parent.OwnerDocument.CreateElement(tagName);
            parent.AppendChild(node);
            if (textContent != null)
            {
                XmlNode content;
                content = parent.OwnerDocument.CreateTextNode(textContent);
                node.AppendChild(content);
            }
            return node;
        }
        public static XmlNode AddAttribute(string attributeName, string textContent, XmlNode parent)
        {
            XmlAttribute attribute;
            attribute = parent.OwnerDocument.CreateAttribute(attributeName);
            attribute.Value = textContent;
            parent.Attributes.Append(attribute);
            return attribute;
        }


        public static void SetElementByName(string xmlFileName, string tagName, string strValue)
        {
            XmlDocument doc = new XmlDocument();
            doc.Load(xmlFileName);
            XmlNodeList nodes = doc.GetElementsByTagName(tagName);
            foreach (XmlNode v in nodes)
            {
                v.InnerText = strValue;
            }
            doc.Save(xmlFileName);
        }

        public static string GetElementByName(string xmlFileName, string tagName)
        {
            try
            {
                string result = null;
                XmlDocument doc = new XmlDocument();
                doc.Load(xmlFileName);
                XmlNodeList nodes = doc.GetElementsByTagName(tagName);
                foreach (XmlNode v in nodes)
                {
                    result = v.InnerText;
                }
                return result;
            }
            catch
            {
                throw new Exception("打开文件错误");
            }
            return null;
        }

        public static int GetElementValueByName(string xmlFileName, string tagName)
        {
            int v = 0;
            try
            {
                v = int.Parse(GetElementByName(xmlFileName, tagName));
            }
            catch (Exception ex)
            {
                throw ex;
            }
            return v;
        }

        public static bool ReadXMLInfoToObject(string xmlFileName, object obj, Type type)
        {
            try
            {
                XElement rootElement = XElement.Load(xmlFileName);

                var elems = rootElement.Elements();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);

                foreach (PropertyDescriptor p in properties)
                {
                    var elm = elems.FirstOrDefault(elem => elem.Name == p.Name);
                    if (elm != null)
                    {
                        if (p.PropertyType == typeof(string))
                        {
                            p.SetValue(obj, elm.Value);
                        }
                        else if (p.PropertyType == typeof(int))
                        {
                            p.SetValue(obj, int.Parse(elm.Value));

                        }
                        else if (p.PropertyType == typeof(float))
                        {
                            p.SetValue(obj, float.Parse(elm.Value));

                        }
                        else if (p.PropertyType == typeof(double))
                        {
                            p.SetValue(obj, double.Parse(elm.Value));

                        }
                        else if (p.PropertyType == typeof(bool))
                        {
                            p.SetValue(obj, int.Parse(elm.Value) == 0 ? false : true);
                        }
                    }
                }
                return true;
            }
            catch (Exception ex)
            {

                return false;
            }

        }

        public static bool SaveXMLInfoToObject(string xmlFileName, object obj, Type type)
        {
            try
            {
                XElement rootElement = XElement.Load(xmlFileName);

                var elems = rootElement.Elements();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);

                foreach (PropertyDescriptor p in properties)
                {
                    var elm = elems.FirstOrDefault(elem => elem.Name == p.Name);
                    if (elm != null)
                    {
                        if (p.PropertyType == typeof(bool))
                        {
                            elm.Value = (bool)p.GetValue(obj) ? "1" : "0";
                        }
                        else
                        {
                            elm.Value = p.GetValue(obj).ToString();
                        }
                    }
                }
                rootElement.Save(xmlFileName);
                return true;
            }
            catch (Exception ex)
            {

                return false;
            }

        }

        public static bool ReadXMLInfoToObject(string xmlFileName, System.Collections.IList data, Type type, string listName)
        {
            try
            {
                data.Clear();

                XElement rootElement = XElement.Load(xmlFileName);
                var elems = rootElement.Elements();

                var lists = elems.First(elem => elem.Name == listName && elem.HasElements);

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);

                foreach (var item in lists.Elements())
                {
                    object obj = System.Activator.CreateInstance(type);

                    foreach (PropertyDescriptor p in properties)
                    {
                        var elm = item.Elements().FirstOrDefault(elem => elem.Name == p.Name);
                        if (elm != null)
                        {
                            if (p.PropertyType == typeof(string))
                            {
                                p.SetValue(obj, elm.Value);
                            }
                            else if (p.PropertyType == typeof(int))
                            {
                                p.SetValue(obj, int.Parse(elm.Value));

                            }
                            else if (p.PropertyType == typeof(float))
                            {
                                p.SetValue(obj, float.Parse(elm.Value));

                            }
                            else if (p.PropertyType == typeof(double))
                            {
                                p.SetValue(obj, double.Parse(elm.Value));

                            }
                            else if (p.PropertyType == typeof(bool))
                            {
                                p.SetValue(obj, int.Parse(elm.Value) == 0 ? false : true);
                            }
                        }
                    }
                    data.Add(obj);

                }

                return true;
            }
            catch (Exception ex)
            {

                return false;
            }

        }

        public static bool ReadXMLInfoToObject(string xmlFileName, System.Collections.IList data, string listName)
        {
            try
            {
                data.Clear();

                XElement rootElement = XElement.Load(xmlFileName);
                var elems = rootElement.Elements();

                var lists = elems.First(elem => elem.Name == listName && elem.HasElements);


                foreach (var item in lists.Elements())
                {
                    data.Add(item.Value);
                }

                return true;
            }
            catch (Exception ex)
            {

                return false;
            }

        }

    }
}
