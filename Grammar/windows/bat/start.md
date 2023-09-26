# 有关 Bat 中 start 命令的使用方法的详细说明、参数列表和示例
## 1. start 命令的使用方法
start 命令用于在新窗口中启动应用程序、命令或批处理文件。使用 start 命令时，可以指定要启动的程序的路径和命令行参数。下面是 start 命令的基本语法：

```bat
START ["title"] [/D path] [/I] [/MIN] [/MAX] [/SEPARATE | /SHARED]
 
     [/LOW | /NORMAL | /HIGH | /REALTIME | /ABOVENORMAL | /BELOWNORMAL]
 
     [/NODE <NUMA node>] [/AFFINITY <hex affinity mask>] [/WAIT] [/B]
 
     [command/program] [parameters]
```
其中，方括号表示可选参数。下面是各个参数的详细说明：

- “title”: 可选参数，指定窗口标题。
- /d path: 可选参数，指定启动程序的工作目录。
- /b 启动应用程序，但不创建新窗口。应用程序已忽略^C处理。除非应用程序启用^C处理，否则^Break是唯一可以中断该应用程序的方式。可选参数，以后台模式启动程序。除非应用程序启用 CTRL+C，否则将忽略 CTRL+C 操作。使用 CTRL+BREAK 中断应用程序。
- /i: 可选参数，以新窗口中的分离模式启动程序,新的环境将是传递给cmd.exe的原始环境，而不是当前环境。。
- /min: 可选参数，启动程序时最小化窗口。
- /max: 可选参数，启动程序时最大化窗口。
- /separate | /shared: 可选参数，指定启动程序时使用的窗口类型。/separate 表示在新的独立窗口中启动程序（默认值），/shared 表示在共享控制台窗口中启动程序。
- /low | /normal | /high | /realtime: 可选参数，指定启动程序时使用的进程优先级。/low 表示低优先级，/normal 表示正常优先级（默认值），/high 表示高优先级，/realtime 表示实时优先级。
- /wait: 可选参数，等待程序结束后再退出。
- ABOVENORMAL 在ABOVENORMAL优先级类中启动应用程序。
- BELOWNORMAL   在BELOWNORMAL 优先级类中启动应用程序。
- NODE   将首选非一致性内存结构(NUMA)节点指定为十进制整数。
- AFFINITY    将处理器关联掩码指定为十六进制数字。进程被限制在这些处理器上运行。当/AFFINITY 和 /NODE 结合时，会对关联掩码进行不同的解释。指定关联掩码，正如 NUMA 节点的处理器掩码正确移动到零位起始位置一样。进程被限制在指定关联掩码和 NUMA 节点之间的那些通用处理器上运行。如果没有通用处理器，则进程被限制在指定的 NUMA 节点上运行。
- command/program 如果它是内部cmd命令或批文件，则该命令处理器是使用cmd.exe的/K开关运行的。这表示运行该命令之后，该窗口将仍然存在。如果它不是内部 cmd 命令或批文件，则它就是一个程序，并将作为一个窗口化应用程序或控制台应用程序运行。
- parameters  这些是传递给 command/program 的参数。

注意: 在 64 位平台上不支持 SEPARATE 和 SHARED 选项。
## 2. 示例
下面是一些示例，演示如何使用 start 命令启动不同类型的应用程序：

- 启动记事本：

```bat
start notepad.exe
```

- 启动计算器：
```bat
start calc.exe
```
启动 Internet Explorer：
```bat
start iexplore.exe "http://www.example.com"
```
- 启动 Windows Media Player：
```bat
start wmplayer.exe "C:\Users\UserName\Music\Sample.mp3"
```
- 启动命令提示符：
```bat
start cmd.exe
```
- 启动 PowerShell：
```bat
start powershell.exe
```
除了启动应用程序外，start 命令还可以启动批处理文件。例如，可以创建一个名为 “test.bat” 的批处理文件，其中包含以下命令：

```bat
start notepad.exe
```
然后在命令提示符中执行以下命令来运行该批处理文件：
```bat
test.bat
```
这将启动记事本程序。
