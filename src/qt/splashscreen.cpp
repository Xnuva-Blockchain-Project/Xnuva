// Copyright (c) 2011-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin-build-config.h> // IWYU pragma: keep

#include <qt/splashscreen.h>

#include <clientversion.h>
#include <common/system.h>
#include <interfaces/handler.h>
#include <interfaces/node.h>
#include <interfaces/wallet.h>
#include <qt/guiutil.h>
#include <qt/networkstyle.h>
#include <qt/walletmodel.h>
#include <util/translation.h>

#include <functional>

#include <QApplication>
#include <QCloseEvent>
#include <QPainter>
#include <QRadialGradient>
#include <QScreen>


SplashScreen::SplashScreen(const NetworkStyle* networkStyle)
    : QWidget()
{
    float devicePixelRatio = static_cast<QGuiApplication*>(
        QCoreApplication::instance())->devicePixelRatio();

    QString titleText = CLIENT_NAME;
    QString versionText = QString("Version %1").arg(
        QString::fromStdString(FormatFullVersion()));
    const QString& titleAddText = networkStyle->getTitleAddText();

    const QSize logicalSize(960, 540);
    const QSize pixelSize(
        qRound(logicalSize.width() * devicePixelRatio),
        qRound(logicalSize.height() * devicePixelRatio));

    pixmap = QPixmap(pixelSize);
    pixmap.setDevicePixelRatio(devicePixelRatio);

    QPixmap artwork(":/icons/xnuva_splash");

    if (artwork.isNull()) {
        pixmap.fill(Qt::black);
    } else {
        QPainter backgroundPainter(&pixmap);
        backgroundPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);

        const QPixmap scaled = artwork.scaled(
            pixelSize,
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation);

        const int x = (scaled.width() - pixelSize.width()) / 2;
        const int y = (scaled.height() - pixelSize.height()) / 2;

        backgroundPainter.drawPixmap(
            QRect(0, 0, pixelSize.width(), pixelSize.height()),
            scaled,
            QRect(x, y, pixelSize.width(), pixelSize.height()));
    }

    QPainter pixPaint(&pixmap);
    pixPaint.setRenderHint(QPainter::Antialiasing, true);

    // The approved concept artwork contains an early static version placeholder.
    // Cover it and render the actual binary version dynamically.
    // Cover the static concept-art version/network placeholder and
    // replace it with the live binary version and live startup status.
    QRect statusPanel(
        qRound(18 * devicePixelRatio),
        qRound(446 * devicePixelRatio),
        qRound(305 * devicePixelRatio),
        qRound(88 * devicePixelRatio));

    pixPaint.fillRect(statusPanel, QColor(5, 13, 26, 225));

    QFont versionFont = QApplication::font();
    versionFont.setPixelSize(qRound(14 * devicePixelRatio));
    versionFont.setBold(true);

    pixPaint.setFont(versionFont);
    pixPaint.setPen(Qt::white);

    QString displayedVersion = versionText;
    if (!titleAddText.isEmpty()) {
        displayedVersion += QString(" ") + titleAddText;
    }

    QRect versionRect = statusPanel.adjusted(
        qRound(12 * devicePixelRatio),
        qRound(4 * devicePixelRatio),
        -qRound(12 * devicePixelRatio),
        -qRound(54 * devicePixelRatio));

    pixPaint.drawText(
        versionRect,
        Qt::AlignLeft | Qt::AlignVCenter,
        displayedVersion);

    pixPaint.end();

    // Set window title
    setWindowTitle(titleText + " " + titleAddText);

    // Resize window and move to center of desktop, disallow resizing
    QRect r(QPoint(), QSize(pixmap.size().width()/devicePixelRatio,pixmap.size().height()/devicePixelRatio));
    resize(r.size());
    setFixedSize(r.size());
    move(QGuiApplication::primaryScreen()->geometry().center() - r.center());

    installEventFilter(this);

    GUIUtil::handleCloseWindowShortcut(this);
}

SplashScreen::~SplashScreen()
{
    if (m_node) unsubscribeFromCoreSignals();
}

void SplashScreen::setNode(interfaces::Node& node)
{
    assert(!m_node);
    m_node = &node;
    subscribeToCoreSignals();
    if (m_shutdown) m_node->startShutdown();
}

void SplashScreen::shutdown()
{
    m_shutdown = true;
    if (m_node) m_node->startShutdown();
}

bool SplashScreen::eventFilter(QObject * obj, QEvent * ev) {
    if (ev->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
        if (keyEvent->key() == Qt::Key_Q) {
            shutdown();
        }
    }
    return QObject::eventFilter(obj, ev);
}

static void InitMessage(SplashScreen *splash, const std::string &message)
{
    bool invoked = QMetaObject::invokeMethod(splash, "showMessage",
        Qt::QueuedConnection,
        Q_ARG(QString, QString::fromStdString(message)),
        Q_ARG(int, Qt::AlignBottom|Qt::AlignLeft),
        Q_ARG(QColor, QColor(235,245,255)));
    assert(invoked);
}

static void ShowProgress(SplashScreen *splash, const std::string &title, int nProgress, bool resume_possible)
{
    InitMessage(splash, title + std::string("\n") +
            (resume_possible ? SplashScreen::tr("(press q to shutdown and continue later)").toStdString()
                                : SplashScreen::tr("press q to shutdown").toStdString()) +
            strprintf("\n%d", nProgress) + "%");
}

void SplashScreen::subscribeToCoreSignals()
{
    // Connect signals to client
    m_handler_init_message = m_node->handleInitMessage([this](const std::string& message) {
        InitMessage(this, message);
    });
    m_handler_show_progress = m_node->handleShowProgress([this](const std::string& title, int nProgress, bool resume_possible) {
        ShowProgress(this, title, nProgress, resume_possible);
    });
    m_handler_init_wallet = m_node->handleInitWallet([this]() { handleLoadWallet(); });
}

void SplashScreen::handleLoadWallet()
{
#ifdef ENABLE_WALLET
    if (!WalletModel::isWalletEnabled()) return;
    m_handler_load_wallet = m_node->walletLoader().handleLoadWallet([this](std::unique_ptr<interfaces::Wallet> wallet) {
        m_connected_wallet_handlers.emplace_back(wallet->handleShowProgress([this](const std::string& title, int nProgress) {
            ShowProgress(this, title, nProgress, /*resume_possible=*/false);
        }));
        m_connected_wallets.emplace_back(std::move(wallet));
    });
#endif
}

void SplashScreen::unsubscribeFromCoreSignals()
{
    // Disconnect signals from client
    m_handler_init_message->disconnect();
    m_handler_show_progress->disconnect();
    for (const auto& handler : m_connected_wallet_handlers) {
        handler->disconnect();
    }
    m_connected_wallet_handlers.clear();
    m_connected_wallets.clear();
}

void SplashScreen::showMessage(const QString &message, int alignment, const QColor &color)
{
    curMessage = message;
    curAlignment = alignment;
    curColor = color;
    update();
}

void SplashScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
    // Keep live initialization/progress text inside the bottom-left
    // status panel built into the Xnuva splash composition.
    QRect r(30, 476, 275, 50);

    QFont statusFont = font();
    statusFont.setPixelSize(13);
    painter.setFont(statusFont);
    painter.setPen(curColor);
    painter.drawText(r, curAlignment | Qt::TextWordWrap, curMessage);
}

void SplashScreen::closeEvent(QCloseEvent *event)
{
    shutdown(); // allows an "emergency" shutdown during startup
    event->ignore();
}
