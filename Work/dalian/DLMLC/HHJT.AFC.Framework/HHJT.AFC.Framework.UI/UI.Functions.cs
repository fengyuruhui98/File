using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using HHJT.AFC.Base.API;

namespace HHJT.AFC.Framework.UI
{
    public enum ProjectPics
    {
        DialogIcon,
        AppIcon,
    }

    public interface IProjectInfo
    {
        string GetPicUriString(ProjectPics picIndex);
        Uri GetPicUri(ProjectPics picIndex);
    }


    public static class HHJT_AFC_UI_Functions
    {
        private static IProjectInfo _projectInfo;

        public static IProjectInfo ProjectInfo
        {
            get { return HHJT_AFC_UI_Functions._projectInfo; }
            set { HHJT_AFC_UI_Functions._projectInfo = value; }
        }

        public static ImageSource GetProjectImageSource()
        {
            return new ImageSourceConverter().ConvertFromString(_projectInfo.GetPicUriString(ProjectPics.DialogIcon)) as ImageSource;
        }

        public static SolidColorBrush GetBrushFromString(string colorName)
        {
            return (SolidColorBrush)System.ComponentModel.TypeDescriptor.GetConverter(typeof(SolidColorBrush)).ConvertFromInvariantString(colorName);
        }

        public static BitmapImage GetBitmapImageFromBytes(byte[] photoData)
        {
            MemoryStream ms = new MemoryStream(photoData);
            ms.Seek(0, System.IO.SeekOrigin.Begin);

            BitmapImage bmp = new BitmapImage();

            bmp.BeginInit();

            bmp.StreamSource = ms;

            bmp.EndInit();

            return bmp;

        }

        public static BitmapImage GetBitmapImageFromFile(string strFile)
        {
            byte[] data = null;

            try
            {
                FileOP.ReadBinaryFile(strFile, out data);
            }
            catch (Exception)
            {

            }

            return (data != null) ? GetBitmapImageFromBytes(data) : null;
        }

        public static bool IsNotNullExist(params object[] objs)
        {
            foreach (var item in objs)
            {
                if (item != null) return true;
            }
            return false;
        }
    }

}
