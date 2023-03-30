using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Excel = Microsoft.Office.Interop.Excel;

namespace HHJT.AFC.Tools.Develop
{
    class ExcelOP
    {
        private string _fileName;

        public ExcelOP(string fileName)
        {
            _fileName = fileName;
        }

        public ObservableCollection<string> _sheets = new ObservableCollection<string>();


        public ObservableCollection<string> GetExcelSheets()
        {
            _sheets.Clear();
            Excel.Application xlApp = new Excel.Application();
            Excel.Workbook xlWorkbook = xlApp.Workbooks.Open(_fileName);

            Excel.Worksheet xlWorksheet;
            foreach (var item in xlWorkbook.Sheets)
            {
                xlWorksheet = item as Excel.Worksheet;
                _sheets.Add(xlWorksheet.Name);
            }
            xlWorkbook.Close();
            xlApp.Quit();

            return _sheets;
        }


        private string GetCellContent(Excel.Range range, int i, int j)
        {
            dynamic c = range.Cells[i, j].Value2;
            return c == null ? null : c.ToString();
        }

        public string[,] ReadExcelSheet(string sheetName)
        {
            Excel.Application xlApp = new Excel.Application();
            Excel.Workbook xlWorkbook = xlApp.Workbooks.Open(_fileName);
            Excel.Worksheet xlWorksheet = xlWorkbook.Sheets[sheetName];

            Excel.Range xlRange = xlWorksheet.UsedRange;

            int rowCount = xlRange.Rows.Count;
            int colCount = xlRange.Columns.Count;

            string[,] strMatrix = new string[rowCount, colCount];

            for (int i = 0; i < rowCount; i++)
            {
                for (int j = 0; j < colCount; j++)
                {
                    strMatrix[i, j] = GetCellContent(xlRange, i + 1, j + 1);
                }
            }
            xlWorkbook.Close();
            xlApp.Quit();

            return strMatrix;
        }


    }
}
