﻿#pragma checksum "..\..\UI.DataGirdPagging.xaml" "{8829d00f-11b8-4213-878b-770e8597ac16}" "2C4B64F4643635197D806C4B5BE359C84A8D886E97C0EDAEFF4030C87DF36901"
//------------------------------------------------------------------------------
// <auto-generated>
//     此代码由工具生成。
//     运行时版本:4.0.30319.42000
//
//     对此文件的更改可能会导致不正确的行为，并且如果
//     重新生成代码，这些更改将会丢失。
// </auto-generated>
//------------------------------------------------------------------------------

using HHJT.AFC.Framework.Skin;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms.Integration;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;


namespace HHJT.AFC.Framework.UI {
    
    
    /// <summary>
    /// UIDataGirdPagging
    /// </summary>
    public partial class UIDataGirdPagging : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        
        #line 9 "..\..\UI.DataGirdPagging.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button btnFirstPage;
        
        #line default
        #line hidden
        
        
        #line 10 "..\..\UI.DataGirdPagging.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button btnPageUp;
        
        #line default
        #line hidden
        
        
        #line 11 "..\..\UI.DataGirdPagging.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button btnPageDown;
        
        #line default
        #line hidden
        
        
        #line 12 "..\..\UI.DataGirdPagging.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button btnEndPage;
        
        #line default
        #line hidden
        
        
        #line 13 "..\..\UI.DataGirdPagging.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.ComboBox cbxPageSizeList;
        
        #line default
        #line hidden
        
        
        #line 14 "..\..\UI.DataGirdPagging.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBlock txtPagerInfo;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/HHJT.AFC.Framework.UI;component/ui.datagirdpagging.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\UI.DataGirdPagging.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            this.btnFirstPage = ((System.Windows.Controls.Button)(target));
            
            #line 9 "..\..\UI.DataGirdPagging.xaml"
            this.btnFirstPage.Click += new System.Windows.RoutedEventHandler(this.btnFirstPage_Click);
            
            #line default
            #line hidden
            return;
            case 2:
            this.btnPageUp = ((System.Windows.Controls.Button)(target));
            
            #line 10 "..\..\UI.DataGirdPagging.xaml"
            this.btnPageUp.Click += new System.Windows.RoutedEventHandler(this.btnPageUp_Click);
            
            #line default
            #line hidden
            return;
            case 3:
            this.btnPageDown = ((System.Windows.Controls.Button)(target));
            
            #line 11 "..\..\UI.DataGirdPagging.xaml"
            this.btnPageDown.Click += new System.Windows.RoutedEventHandler(this.btnPageDown_Click);
            
            #line default
            #line hidden
            return;
            case 4:
            this.btnEndPage = ((System.Windows.Controls.Button)(target));
            
            #line 12 "..\..\UI.DataGirdPagging.xaml"
            this.btnEndPage.Click += new System.Windows.RoutedEventHandler(this.btnEndPage_Click);
            
            #line default
            #line hidden
            return;
            case 5:
            this.cbxPageSizeList = ((System.Windows.Controls.ComboBox)(target));
            
            #line 13 "..\..\UI.DataGirdPagging.xaml"
            this.cbxPageSizeList.SelectionChanged += new System.Windows.Controls.SelectionChangedEventHandler(this.cbxPageSizeList_SelectionChanged);
            
            #line default
            #line hidden
            return;
            case 6:
            this.txtPagerInfo = ((System.Windows.Controls.TextBlock)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

