#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDir>
#include<QFileDialog>
#include<QStandardPaths>
#include<fstream>
#include<iostream>
#include<mydialog.h>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/hexo.png"));
    this->setWindowTitle("hugo博客图形化写作工具——made by L_B__");
    m_Process = new QProcess(this);
    connect(m_Process,SIGNAL(readyReadStandardOutput()),this,SLOT(onStandardOutput()));
    connect(m_Process,SIGNAL(readyReadStandardError()),this,SLOT(onErrorOutput()));
    std::ifstream fd("./BlogPath.txt");//写入博客路径
    std::string tmp_path;
    fd>>tmp_path;
    fd.close();
    if(!tmp_path.empty())
        BlogPath.append(tmp_path.c_str());
    tmp_path.clear();
    fd.open("./github_repo.txt");
    fd>>tmp_path;
    fd.close();
    if(!tmp_path.empty())           //写入GitHub路径
        githuboRepoPath = tmp_path.c_str();
    tmp_path.clear();
    fd.open("./gitee_repo.txt");
    fd>>tmp_path;
    fd.close();
    if(!tmp_path.empty())           //写入gitee路径
        giteeRepoPath = tmp_path.c_str();

    isInput = false; //记录textEdit是否变为可输入状态，如果是，那么肯定处于添加图片的状态，默认不是添加图片的状态
    if(!BlogPath.isEmpty()&&(!githuboRepoPath.isEmpty()||!giteeRepoPath.isEmpty()))
    {
        ui->textEdit->append("当前博客地址已初始化\n");
        ui->textEdit->append(QString(76,'-')+'\n');
        show_Blogpath();
    }
    else
    {
        if(BlogPath.isEmpty()){
            ui->textEdit->append("当前博客尚未完成初始化信息，请点击蓝色按钮完成必要的初始化！\n");
            ui->textEdit->append(QString(76,'-')+'\n');}
        else{
            ui->textEdit->append("本地博客位置已经初始化，但远端尚未初始化。\n建议先在本地写几篇文章调试后再进行远端的初始化！\n");
            ui->textEdit->append(QString(76,'-')+'\n');
            show_Blogpath();
        }
    }
    ui->debug_link->setText(R"(<html>    <style>
                    a {
                        color:#3281b8;
                    }
                </style><head/><body><p>本地调试链接：<a href="http://localhost:1313/">localhost:1313</a></p></body></html>)");
    ui->video_link->setText(R"(<html>    <style>
                    a {
                        color:#3281b8;
                    }
                </style><head/><body><p><a href="https://www.bilibili.com/video/BV11S4y1G7SW">视频教程</a></p></body></html>)");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QList<QString> MainWindow::GetProcessIDByPort(uint port)
{
    QProcess qp;
    qp.start("netstat",QStringList()<<"-ano");
    QString str;
    if(qp.waitForFinished()){
        str = qp.readAll();
    }
    QList<QString>ret;
    std::stringstream ss;   //用标准库的stringstream进行处理，就不直接split了
    std::string tmp;
    ss<<str.toStdString();
    std::string str_port = std::to_string(port);
    int i;
    while(std::getline(ss,tmp,'\r')){
        //检查是否包含需要的端口号，如果包含，则进一步判断是否在端口号字段（前面一个字符是':'，后面是空格）
        if((i=tmp.find(str_port))!=-1){
            if(i+str_port.size()<tmp.size()&&tmp[i+str_port.size()]==' '&&i-1>=0&&tmp[i-1]==':'){
                ret.append(tmp.c_str());
            }
        }
    }
    qp.close();
    return ret;
}

void MainWindow::onStandardOutput()
{
    QByteArray qbt = m_Process->readAllStandardOutput();
    QString msg = QString::fromLocal8Bit(qbt);
    ui->textEdit->append(msg);
    ui->textEdit->update();
}

void MainWindow::onErrorOutput()
{
    QByteArray qbt = m_Process->readAllStandardError();
    QString msg = QString::fromLocal8Bit(qbt);
    ui->textEdit->append(msg);
    ui->textEdit->update();
}

//专门执行cmd命令
void MainWindow::startCmd(const QString& qcmd)
{
    ui->textEdit->clear();

    m_Process->startCommand(qcmd,QProcess::ReadWrite);
}


void MainWindow::on_choose_pos_clicked()
{
    setBtnFromState(false);
    QString path = QFileDialog::getExistingDirectory(this,"更新博客所在路径",
                                                     QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if(path.isEmpty())
        return;
    BlogPath = path;
    std::ofstream out("./BlogPath.txt");
    out<<path.toStdString();
    out.close();
    ui->textEdit->append("Blog Path current :");
    show_Blogpath();
}


void MainWindow::on_show_categories_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    setBtnFromState(false);
    startCmd("./QtRun -sc");
}

void MainWindow::show_Blogpath()
{
    ui->textEdit->append(QString("hugo博客所在地：")+BlogPath+"\n");
    if(!githuboRepoPath.isEmpty()){
        ui->textEdit->append(QString("Github远程仓库：")+githuboRepoPath+"\n" );
    }
    else{
        ui->textEdit->append(QString("Github远程仓库：")+"暂未选择"+"\n" );
    }
    if(!giteeRepoPath.isEmpty()){
        ui->textEdit->append(QString("Gitee远程仓库：")+giteeRepoPath+"\n");
    }
    else{
        ui->textEdit->append(QString("Gitee远程仓库：")+"暂未选择"+"\n");
    }
}


void MainWindow::on_start_write_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    setBtnFromState(false);
    on_show_categories_clicked();
    myDialog input;
    input.setTip("注意输入不能含有空格");
    input.exec();
    if(!input.getCurState())
        return;
    QString text1 = input.getLine1Text();
    QString text2 = input.getLine2Text();
    if(text1.isEmpty()||text2.isEmpty()||text1.contains(' ')||text2.contains(' ')){
        QMessageBox::warning(nullptr,"提示","输入非法");
        return;
    }
    QString cmd = "./QtRun " + text1 + ' ' + text2;
    startCmd(cmd);
}


void MainWindow::on_show_Blog_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    setBtnFromState(false);
    ui->textEdit->clear();
    show_Blogpath();
    ui->textEdit->update();
}


void MainWindow::on_debug_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    setBtnFromState(false);
    std::ofstream writer("./hugoDebug.bat");
    if(!writer.is_open())
        return;
    std::string lineCmd = "cd /d";
    lineCmd += BlogPath.toStdString();
    writer<<lineCmd<<'\n';
    lineCmd.clear();
    lineCmd += "hugo";
    writer<<lineCmd<<'\n';
    lineCmd += " server";
    writer<<lineCmd<<'\n';
    writer.close();
    startCmd("./hugoDebug.bat");
}


void MainWindow::on_addImg_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    if(!isInput){
        ui->textEdit->clear();
        setBtnFromState(true);//表示您可以开始输入了
    }else{
        if(ui->textEdit->toPlainText().isEmpty()){//为空提示输入为空
            QMessageBox::information(nullptr,"提示","输入为空");
            return;
        }
        QStringList imgs = ui->textEdit->toPlainText().split('\n');
        std::ofstream writer("./initImg.txt",std::ios::app);
        for(auto& img:imgs){
        if(img.endsWith(".png")||img.endsWith(".jpg")||img.endsWith(".jpeg")||img.endsWith(".bmp")) //如果是图片链接就写入
            writer<<"\n\r"<<img.toStdString();
        }
        writer.close();
        setBtnFromState(false);//回到初始状态，并且不能再输入
        QMessageBox::information(nullptr,"提示","成功添加图片（非图片链接会跳过）");
    }
}

//根据当前信号传递的状态调整按钮的内容
void MainWindow::setBtnFromState(bool f)
{
    if(f){
        isInput = true;
        ui->addImg->setText("完成");
        ui->textEdit->setReadOnly(false);
    }else{
        isInput = false;
        ui->addImg->setText("添加图片");
        ui->textEdit->setReadOnly(true);
    }
}


void MainWindow::on_open_categories_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    std::ofstream writer("./exec_code.bat");
    writer<<"code ./categories.txt";
    writer.close();
    startCmd("./exec_code.bat");
}


void MainWindow::on_open_imgfile_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    std::ofstream writer("./exec_code.bat");
    writer<<"code ./InitImg.txt";
    writer.close();
    startCmd("./exec_code.bat");
}


void MainWindow::on_init_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    myDialog input;
    input.setTip("远端仓库地址");
    input.setLable1Test("GitHub仓库：");
    input.setLable2Test("Gitee仓库：");
    input.setLine1Test("请输入github仓库");
    input.setLine2Test("请输入gitee仓库");
    input.exec();
    if(input.getCurState()){
        auto local_repo = BlogPath + "\\public";
        if(!QDir(local_repo).exists()){
            QMessageBox::warning(nullptr,"错误","public文件夹不存在，请先进行本地调试生成public文件夹" );
            return;
        }
        //写入远程关联仓库地址
        QString repo_github = input.getLine1Text();
        QString repo_gitee = input.getLine2Text();
        if(repo_gitee.isEmpty()&&repo_github.isEmpty()){
            QMessageBox::warning(nullptr,"注意","两个仓库不能都为空");
            return;
        }

        std::ofstream writer;
        if(!repo_github.isEmpty())
        {
            if(!repo_gitee.endsWith(".git")){
                QMessageBox::warning(nullptr,"提示","输入GitHub仓库地址有误");
                return;
            }
            writer.open("./github_repo.txt");
            writer<<repo_github.toStdString();
            writer.close();
            githuboRepoPath = repo_github;
        }
        if(!repo_gitee.isEmpty())
        {
            if(!repo_gitee.endsWith(".git")){
                QMessageBox::warning(nullptr,"提示","输入Gitee仓库地址有误");
                return;
            }
            writer.open("./gitee_repo.txt");
            writer<<repo_gitee.toStdString();
            writer.close();
            giteeRepoPath = repo_gitee;
        }
        //本地git仓库初始化以及和远端仓库的关联
        writer.open("./exec_code.bat");
        auto cmd_cd = QString("cd /d") + local_repo;
        writer<<cmd_cd.toStdString()<<'\n';
        writer<<"git init"<<'\n';
        if(!repo_github.isEmpty()){
            writer<<"git remote remove github\n";
            writer<<"git remote add github "<<repo_github.toStdString()<<'\n';
        }
        if(!repo_gitee.isEmpty()){
            writer<<"git remote remove gitee\n";
            writer<<"git remote add gitee "<<repo_gitee.toStdString()<<'\n';
        }
        writer.close();
        startCmd("./exec_code.bat");
        ui->textEdit->append(QString(76,'-')+'\n');
        ui->textEdit->append("当前博客地址已更新\n");
        show_Blogpath();

    }
}
//git add .
//git commit -m "update"    3 10 33 56 72
//git pull
//git push -u xx master
//git --global --unset http.proxy
//git --global --unset https.proxy
//git --global http.proxy http://localhost:7890
//git --global https.proxy https://localhost:7890

void MainWindow::on_release_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    auto local_repo = BlogPath + "/public";
    if(!QDir(local_repo+"/.git").exists())
    {
        QMessageBox::warning(nullptr,"注意","git仓库未初始化");
        return;
    }
    QMessageBox::information(nullptr,"提示","注意：GitHub仓库提交需要开启代理");
    std::ofstream writer("./exec_code.bat");
    auto cd_cmd = QString("cd /d") + local_repo;
    auto cd_blog = QString("cd /d") + BlogPath;
    writer<<cd_blog.toStdString()<<'\n'<<"hugo\n";
    writer<<cd_cmd.toStdString()<<'\n';
    writer<<"git add ."<<'\n';
    writer<<R"(git commit -m "update")"<<'\n';
    //先进行简单一些的国内仓库提交，这个不需要开代理
    writer<<"git config --global --unset http.proxy"<<'\n';
    writer<<"git config --global --unset https.proxy"<<'\n';
    writer<<"git pull gitee master"<<'\n';
    writer<<"git push -u gitee master"<<'\n';

    //然后开代理提交GitHub
    writer<<"git config --global http.proxy  http://127.0.0.1:7890"<<'\n';
    writer<<"git config --global https.proxy  https://127.0.0.1:7890"<<'\n';

    //注意延时五秒，否则可能出现代理不管用
    writer<<"TIMEOUT /T 5"<<'\n';
    writer<<"git pull github master"<<'\n';
    writer<<"git push -u github master"<<'\n';
    //最后再把代理关上
    writer<<"git config --global --unset http.proxy"<<'\n';
    writer<<"git config --global --unset https.proxy"<<'\n';
    writer.close();

    startCmd("./exec_code.bat");
}


void MainWindow::on_check_processByport_clicked()
{
    setBtnFromState(false);
    myDialog input;
    input.setTip("输入您要查询的端口号");
    input.hide1();
    input.setLable2Test("端口号：");
    input.setLine2Test("请输入端口号");
    input.exec();
    if(!input.getCurState())
        return;
    auto text = input.getLine2Text();
    if(text.isEmpty()){
        QMessageBox::warning(nullptr,"提示","输入为空");
        return;
    }
    if(text.toUInt()==0&&text[0]!='0'){
        QMessageBox::warning(nullptr,"提示","输入非法");
        return;
    }
    auto content = GetProcessIDByPort(text.toUInt());
    if(content.isEmpty()){
        QMessageBox::warning(nullptr,"提示","该端口号暂时没被占用");
        return ;
    }
    ui->textEdit->clear();
    QString first_line;
    first_line += "协议";
    first_line += QString(6,' ');
    first_line += "本地地址";
    first_line += QString(18,' ');
    first_line += "外部地址";
    first_line += QString(18,' ');
    first_line += "状态";
    first_line += QString(16,' ');
    first_line += "PID";
    ui->textEdit->append(first_line);
    QString second_line(first_line.size(),'-');
    ui->textEdit->append(second_line);
    for(auto& c:content){
        ui->textEdit->append(c);
    }
    ui->textEdit->update();
}


void MainWindow::on_kill_porcessbyPID_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    setBtnFromState(false);
    myDialog input;
    input.setTip("输入您要杀死进程的PID");
    input.hide1();
    input.setLable2Test("PID：");
    input.setLine2Test("请输入PID");
    input.exec();
    if(!input.getCurState())
        return;
    auto text = input.getLine2Text();
    if(text.isEmpty()){
        QMessageBox::warning(nullptr,"提示","输入为空");
        return;
    }
    if(text.toUInt()==0&&text[0]!='0')
    {
        QMessageBox::warning(nullptr,"提示","输入非法");
        return;
    }
    QString qcmd = QString("taskkill /f /t /im %1").arg(text.toUInt());
    startCmd(qcmd);
}


void MainWindow::on_setEditor_clicked()
{
    m_Process->kill();
    m_Process->waitForFinished(3000);
    myDialog input;
    input.hide2();
    input.setTip("用于写作的编辑器路径");
    input.setLable1Test("编辑器路径");
    input.setLine1Test("请输入编辑器路径");
    input.exec();
    if(input.getCurState()){
        auto ed_path = input.getLine1Text();
        if(!QFile(ed_path).exists()){//如果这个软件地址不存在，那么就是无效地址，给出提示
            QMessageBox::warning(nullptr,"提示","文件路径不存在");
            return;
        }
        std::ofstream writer("./ed_Path.txt");
        if(!writer.is_open()){
            QMessageBox::warning(nullptr,"提示","ed_Path文件写入失败");
            return;
        }
        writer<<ed_path.toStdString();
        writer.close();
        QMessageBox::information(nullptr,"提示","路径更新成功");
    }
}

