#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QTextStream>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonOpen, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->showedit, SIGNAL(clicked()), this, SLOT(showEdit()));

    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setColumnWidth(0, 400);
    setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    showMaximized();
}
void Dialog::showEdit()
{
   QString strText = ui->textEdit->toPlainText();
   QTextStream cout(stdout, QIODevice::WriteOnly);
   strText.remove(QRegExp("\\s"));
   char textinput[10000] = {0};
   QByteArray ba = QByteArray::fromHex (strText.toLatin1().data());//按十六进制编码接入文本

   //QByteArray ba = strText.toLatin1();
   const char *p = textinput;
   p = ba.data();
   printf("ba.data=%d\n", p[0]);
   cout<<"strText == "<<ba.data()<<endl;


   QString strDis;
   QString str3 = ba.toHex().data();//以十六进制显示
   str3 = str3.toUpper ();//转换为大写
   for(int i = 0;i<str3.length ();i+=2)//填加空格
   {
          QString st = str3.mid (i,2);
          strDis += st;
          strDis += " ";
    }
    ui->textEdit_2->append(strDis);

}
void Dialog::openXML(QString fileName)
{
    QString file_name  = "d:/xmltree/xmlfile/test1.xml";
    QFile file(file_name);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom("WCM");
        QString error;
        int row = 0, column = 0;
        if(dom.setContent(&file, false, &error, &row, &column))
        {
            printf("enter openxml\n");
            ui->treeWidget->clear();
            QDomElement docElem = dom.documentElement();
            listDom(docElem, NULL);
        }
    }
    file.close();
}

void Dialog::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"), "d:/", tr("XML Files (*.xml)"));
    printf("enterfirst\n");
    if(!fileName.isEmpty())
    {
        printf("enter openfile");
        openXML( fileName );
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::listDom(QDomElement& docElem, QTreeWidgetItem * pItem)
{
    QDomElement person = docElem.firstChildElement();
    if(person.isNull())
        return ;

    QString person_tag_name = person.tagName();

    //id为1
    //QString id = person.attributeNode("id").value();
    //获取子节点，数目为2
    //QString tag_value = person.text();

    QDomNodeList list = docElem.childNodes();
    int count = list.count();
    for(int i=0; i <count; ++i)
    {
        QDomNode dom_node = list.item(i);
        QDomElement element = dom_node.toElement();
        //QString child__tag_value = element.text();
        if(element.tagName() == "desc" || element.tagName() == "code")
       {
        QTreeWidgetItem *item;
        if(element.tagName() == "desc"){

        if( pItem )
        {
            item = new QTreeWidgetItem(pItem);
        }
        else
        {
            item = new QTreeWidgetItem(ui->treeWidget);
        }
        }
        //item->setText(0, element.tagName());
        if(element.tagName() == "desc")
        item->setText(0, element.text());

        this->listDom(element, item);
        //if(element.tagName() == "desc"){
        if( pItem )
        {
            pItem->addChild(item);
        }
        else
        {
            ui->treeWidget->addTopLevelItem(item);

        }
        //}

    }
}

    /*
    QDomNode node = docElem.firstChild();
    if(node.toElement().isNull())
    {
        pItem->setText (1, docElem.text());
    }
    while(!node.isNull())
    {
        QDomElement element = node.toElement(); // try to convert the node to an element.
        if( !element.isNull() )
        {
            QTreeWidgetItem *item;
            if( pItem )
            {
                item = new QTreeWidgetItem(pItem);
            }
            else
            {
                item = new QTreeWidgetItem(ui->treeWidget);
            }
            item->setText(0, element.tagName());

            listDom(element, item);
            if( pItem )
            {
                pItem->addChild(item);
            }
            else
            {
                ui->treeWidget->addTopLevelItem(item);

            }
        }

        node = node.nextSibling();
    } */
    return;
}
