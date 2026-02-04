# 写在开头
在大一的时候，我作为一个懵懂的小登，总是因为各种编程作业忙的焦头烂额，最后发现没有学会什么，~~写的也是依托答辩~~。
我不禁思考，作为一个计算机科班的学生，花大量的时间在学习老式cpp的实现上，对现代cpp的新特性不闻不问，最后再用一个不算充裕的时间写一些没什么用的项目，到底有什么意义？这样学到的知识如何应用在真正的工程实践上？
当然，我并不是说这种课程毫无意义，根据我的观察，计算机专业的很大部分人在上大学前是根本没有接触过编程的，甚至不会合理地使用电脑。对于他们来说，打好基础固然是重要的，但我认为花费太多时间在这上面是低性价比的，这也是我将我的作业和课设开源的原因：**我希望后人能站在前人的肩膀上，探索更广阔的天空，而不是重复造轮子，在没有意义的事上内耗。**
**代码仅供参考，请勿无脑复制。**这不仅会导致过不了查重，也对个人成长毫无意义。

# 程序设计实践

- [HarryPotterSearch](HarryPotterSearch/)

> 将哈利波特的7本书（txt格式）读入，然后在指定了人名/地名后，显示查询结果，选择指定查询结果序号（选择查询内容），能够显示指定查询结果所在位置前后的一段文字。

基于Qt 6.9.0开发的图形化应用。其他信息请参考[docx文件](HarryPotterSearch/HarryPotterSearch.docx)<br>
<br>
**注意事项**：txt文件应该放入与exe文件同级文件夹中，否则读取时的路径应修改为绝对路径。

- [PlaneGame](PlaneGame/)

一个简易的控制台程序，没啥好说的，这个不是重点，重点是下面的Qt_PlaneGame。

- [Qt_PlaneGame](Qt_PlaneGame/)

基于Qt 6.9.0开发的图形化应用。[QtPlaneGameSourceCode/images](Qt_PlaneGame/QtPlaneGameSourceCode/images/)文件夹内的图片可替换为你的背景，但如果命名不同应修改[qrc文件](Qt_PlaneGame/QtPlaneGameSourceCode/planegame.qrc)。其他信息请参考[docx文件](Qt_PlaneGame/Qt_PlaneGame.docx.docx)。

- [Qt_Bank](Qt_Bank/)

基于Qt 6.9.0开发的图形化应用。其他信息请参考[docx文件](Qt_Bank/Qt_bank.docx)。

参考成绩：<br>
![programming_practice_score](images/programming_practice_score.png)

# 小学期——计算机实践
> 不确定后面的课设要求是否相同，但应该不会太难，能通过程序设计实践的对此应该不会太头疼。

[课设：股票管理系统](Stock/)

主要学习内容：
- Test Driven Development(TDD)开发
- Git版本管理
- ~~水课摸鱼技巧~~

总之不算太难，其他信息请参考[docx文件](Stock/Stock.docx)和[pptx文件](Stock/Stock.pptx)。

参考成绩：<br>
![computer_practice](images/computer_practice.png)
# TO BE CONTINUED...