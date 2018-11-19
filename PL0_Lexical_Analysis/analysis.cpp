#include "analysis.h"
#include "ui_analysis.h"

Analysis::Analysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Analysis)
{
    this->iter = 0;
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setMinimumSize(800,500);
    setWindowTitle("PLO_Lexical_Analysis");
}

Analysis::~Analysis()
{
    delete ui;
}

void Analysis::resizeEvent(QResizeEvent *event){
    int h = this->geometry().height();
    int w = this->geometry().width();
    ui->tableWidget->setGeometry(30,30,w-50,h-80);
    ui->oKButton->move(w-100,h-40);
}

void Analysis::fromMainWindow(QString str){
    this->code = str;
    this->code.replace(QRegularExpression("\\s")," ");
    analysisCode();
}

void Analysis::setItem(QString s1, QString s2, QString s3)
{

    QTableWidgetItem *item = new QTableWidgetItem(s1);
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    int row = ui->tableWidget->rowCount();
    row--;
    ui->tableWidget->setItem(row,0,item);
    item = new QTableWidgetItem(s2);
    ui->tableWidget->setItem(row,1,item);
    item = new QTableWidgetItem(s3);
    ui->tableWidget->setItem(row,2,item);

}

QString Analysis::getIEEEFloat(double n){
    QString str = "";
    unsigned char *p;
    p = (unsigned char *)&n;
    for(int i = 0; i < 8; i++){
        unsigned char a = *p;
        str += QString::number(a,2);
        p++;
    }
    return str;
}
void Analysis::analysisCode()
{
    int length = this->code.length();
    QString id = "";
    bool flag = false;
    for(int i = iter; i < length; i++)
    {
        iter = i;
        flag = true;
        if(id=="")
        {
            if (this->code[i] == 'b' && isbegin()){
                this->setItem("begin","keyword","begin");
                i += 4;
                continue;
            }
            else if(this->code[i] == 'c'){
                if(iscall()){
                    this->setItem("call","keyword","call");
                    i += 3;
                    continue;
                }
                else if(isconst()){
                    this->setItem("const","keyword","const");
                    i+=4;
                    continue;
                }
                else {
                    flag = false;
                }
            }
            else if(this->code[i] == 'd' && isdo()){
                this->setItem("do","keyword","do");
                i+=1;
                continue;
            }
            else if (this->code[i] == 'e') {
                if(iselse()){
                    this->setItem("else","keyword","else");
                    i+=3;
                    continue;
                }
                else if (isend()) {
                    this->setItem("end","keyword","end");
                    i+=2;
                    continue;
                }
                else {
                    flag = false;
                }
            }
            else if (this->code[i]== 'i' && isif()) {
                this->setItem("if","keyword","if");
                i+=1;
                continue;
            }
            else if (this->code[i] == 'o' && isodd()) {
                this->setItem("odd","keyword","odd");
                i+=2;
                continue;
            }
            else if (this->code == 'p' && isprocedure()) {
                this->setItem("procedure","keyword","procedure");
                i+=8;
                continue;
            }
            else if (this->code == 'r') {
                if(isread()){
                    this->setItem("read","keyword","read");
                    i+=3;
                    continue;
                }
                else if (isrepeat()) {
                    this->setItem("repeat","keyword","repeat");
                    i+=5;
                    continue;
                }
                else {
                    flag = false;
                }
            }
            else if (this->code[i] == 't' && isthen()) {
                this->setItem("then","keyword","then");
                i+=3;
                continue;
            }
            else if (this->code[i] == 'u' && isuntil()) {
                this->setItem("until","keyword","until");
                i+=4;
                continue;
            }
            else if (this->code[i] == 'v' && isvar()) {
                this->setItem("var","keyword","var");
                i+=2;
                continue;
            }
            else if (this->code[i] == 'w') {
                if(iswhile()){
                    this->setItem("while","keyword","while");
                    i+=4;
                    continue;
                }
                else if (iswrite()) {
                    this->setItem("write","keyword","write");
                    continue;
                }
                else {
                    flag = false;
                }
            }
            else if (this->code[i].isLetter()) {
                flag = false;
            }
            else if(this->code[i].isNumber()){
                double t = isNum();
                if(t >= 0){
                    QString value = getIEEEFloat(t);
                    this->setItem(QString::number(t,'g',iter - i + 1),"constant",value);
                    i = iter;
                    continue;
                }
                else{
                    QMessageBox::warning(this,"Warning","Identifier cannot start with a number!",QMessageBox::Button::Yes);
                    this->setItem(this->code.mid(i,3),"error","error");
                    return;
                }
            }
        }
        else if(this->code[i].isLetterOrNumber()){
            flag = false;
        }

        if(flag == false){
            if(id == ""){
                if(this->code[i].isLetter())
                    id += this->code[i];
            }
            else {
                id += this->code[i];
            }
        }
        else {
            if(id != ""){
                setItem(id,"identifier",id);
                id = "";
            }
            if(this->code[i] == '='){
                setItem("=","operator","=");
            }
            else if (this->code[i] == ';') {
                setItem(";","delimiter",";");
            }
            else if (this->code[i] == '/') {
                setItem("/","operator","/");
            }
            else if (this->code[i] == '(') {
                setItem("(","operator","(");
            }
            else if (this->code[i] == ')'){
                setItem(")","operator",")");
            }
            else if (this->code[i] == '-') {
                setItem("-","operator","-");
            }
            else if (this->code[i] == '+') {
                setItem("+","operator","+");
            }
            else if (this->code[i] == '*') {
                setItem("*","operator","*");
            }
            else if (this->code[i] == '<'){
                if(this->code[i + 1] == '>'){
                    setItem("<>","operator","<>");
                    i++;
                }
                else if (this->code[i + 1] == '=') {
                    setItem("<=","operator","<=");
                    i++;
                }
                else{
                    setItem("<","operator","<");
                }
            }
            else if (this->code[i] == '>') {
                if(this->code[i + 1] == "="){
                    setItem(">=","operator",">=");
                    i++;
                }
                else{
                    setItem(">","operator",">");
                }
            }
            else if (this->code[i] == ':' && this->code[i + 1] == '='){
                setItem(":=","operator",":=");
                i++;
            }
            else if (this->code[i] == ',') {
                setItem(",","delimiter",",");

            }
            else if (this->code[i] == ' ') {
                continue;
            }
            else{
                QString str = "";
                str += this->code[i];
                setItem(str,"unknow character",str);
            }
        }
    }
}

bool Analysis::isconst()
{
    if(this->code.indexOf("const",this->iter) == iter && (!this->code[iter+5].isLetter()))
    {
        return true;
    }
    return false;
}

bool Analysis::isvar()
{
    if(this->code.indexOf("var",this->iter) == iter && !this->code[iter + 3].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::isif()
{
    if(this->code.indexOf("if",this->iter) == iter && !this->code[iter+2].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::isthen()
{
    if(this->code.indexOf("then",this->iter) == iter && !this->code[iter +4].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::iselse()
{
    if(this->code.indexOf("else",this->iter) == iter && !this->code[iter + 4].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::iswhile()
{
    if(this->code.indexOf("while",this->iter) == iter && !this->code[iter + 5].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::isdo()
{
    if(this->code.indexOf("do",this->iter) == iter && !this->code[iter + 2].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::iscall()
{
    if(this->code.indexOf("call",this->iter) == iter && !this->code[iter+4].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::isbegin()
{
    if(this->code.indexOf("begin",this->iter) == iter && !this->code[iter + 5].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::isend()
{
    if(this->code.indexOf("end",this->iter) == iter && !this->code[iter + 3].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::isrepeat()
{
    if(this->code.indexOf("repeat",this->iter) == iter && !this->code[iter+6].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::isuntil()
{
    if(this->code.indexOf("until",this->iter) == iter && !this->code[iter+5].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::isread()
{
    if(this->code.indexOf("read",this->iter) == iter && !this->code[iter+4].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::iswrite()
{
    if(this->code.indexOf("write",this->iter) == iter && !this->code[iter+5].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::isodd()
{
    if(this->code.indexOf("odd",this->iter) == iter && !this->code[iter+3].isLetter())
    {
        return true;
    }
    return false;
}

bool Analysis::isprocedure()
{
    if(this->code.indexOf("procedure",this->iter) == iter && !this->code[iter+9].isLetter())
    {
        return true;
    }
    return false;
}

double Analysis::isNum()
{
    bool flag = false;
    int i = iter;
    QString num;
    int length = code.length();
    while ((this->code[i].isNumber() || (this->code[i] == '.' && flag== false) )&& i < length) {
        num += this->code[i];
        if(this->code[i] == '.'){
            flag = true;
        }
        i++;
    }
    if(i == length || (this->code[i] == ' ' || this->code[i] == ';' || this->code[i] == '/'
            || this->code[i] == '(' || this->code[i] == ')' || this->code[i] == '-'
            || this->code[i] == '+' || this->code[i] == '*' || this->code[i] == '<'
            || this->code[i] == '>' || this->code[i] == ',')){
        iter = i - 1;
        return num.toDouble();
    }
    else{
        return -1;
    }
}


void Analysis::on_oKButton_clicked()
{
    emit toMainWindow();
    delete this;
}
