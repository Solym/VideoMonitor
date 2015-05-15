#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);//������������ʾ�����ز˵���
	ui->centralWidget->setTabsClosable(false);

	m_ucam = new CUSBCamera(320,240);
	m_ucam->StartGrab();
	m_ucam->StartProc();

	m_previw = new preview(m_ucam, this);
	m_previwindex = ui->centralWidget->addTab(m_previw, tr("֪ͨ"));

	this->move((desktop.availableGeometry().width()-this->width()),desktop.availableGeometry().height());//��ʼ��λ�õ����½�

	isShow = false;
	animation=new QPropertyAnimation(this,"pos");

	m_timer = new QTimer(this);
	m_timer->start(40);
	connect( m_timer, SIGNAL(timeout()), this, SLOT(TimeDone()));
}

MainWindow::~MainWindow()
{
	m_ucam->StopProc();
	m_ucam->StopGrab();
	delete m_ucam;
	delete m_previw;
	delete m_timer;
	delete animation;
	animation=NULL;
}

//��������
void MainWindow::showAnimation()
{
	//��ʾ�����򶯻�
	animation->setDuration(1000);
	animation->setStartValue(QPoint(this->x(),this->y()));
	animation->setEndValue(QPoint((desktop.availableGeometry().width()-this->width()),(desktop.availableGeometry().height()-this->height())));
	animation->start();
}
//�رն���
void MainWindow::closeAnimation()
{
	//�������ȥ����
	animation->setDuration(2000);
	animation->setStartValue(QPoint(this->x(),this->y()));
	animation->setEndValue(QPoint((desktop.availableGeometry().width()-this->width()),desktop.availableGeometry().height()));
	animation->start();
}

void MainWindow::TimeDone()
{
	if(m_ucam->GetShowStatus() && !isShow)
	{
		showAnimation(); //��ʼ��ʾ���½ǵ�����
	}
	if(!m_ucam->GetShowStatus() && isShow)
	{
		closeAnimation();
		m_ucam->ClcCountNum();
	}
	isShow = m_ucam->GetShowStatus();
}
