#include "canvas.h"
#include <QFileInfo>
#include <QPaintEvent>
#include <QFontDatabase>
#include <QAudioDevice>
#include <QMediaDevices>

Canvas::Canvas()
{

    QFileInfo file{__FILE__};
    QString resPath = file.absolutePath() + "/../res/";
    QFontDatabase::addApplicationFont(resPath + "font.ttf");

    blinkersound.setAudioDevice(QMediaDevices::defaultAudioOutput());
    blinkersound.setSource(QUrl::fromLocalFile(resPath + "turn-signals.wav"));
    blinkersound.setLoopCount(QSoundEffect::Infinite);
    blinkersound.setVolume(1.0f);

    brush = QBrush(QColor(0xa6, 0x2e, 0x39));
}

void Canvas::paintEvent(QPaintEvent *event)
{

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(QColor(64, 32, 64)));
    painter.translate(40, 40);

    fontSpeedometer();
    fontBatteryLevel();
    fontBlinkers();
    fontTmpLevel();

    painter.end();
}

void Canvas::fontSpeedometer(void)
{
    int mainValue = 320;
    int centerX = mainValue;
    int centerY = mainValue;
    int radius = mainValue;

    int startAngle = -33 * 16;
    int spanAngle = 246 * 16;
    painter.setPen(QPen(Qt::white, 8, Qt::SolidLine, Qt::FlatCap));
    painter.drawArc(centerX - radius, centerY - radius, radius * 2, radius * 2, startAngle, spanAngle);

    // Draw digital speedometer
    if (status == true)
    {
        painter.setFont(QFont("Arial", 25));
        painter.drawText(QRect(245, 435, 150, 100), Qt::AlignCenter, QString::number(speed) + " km/h");

        iconFont = QFont{"Material Icons", 45, QFont::Normal};
        painter.setPen(QColor(255, 255, 255));
        painter.setFont(iconFont);
        painter.drawText(QRect(270, 390, 100, 100), Qt::AlignCenter, QChar(0xe9e4));
    }
    else
    {

        QString SpeedText = "Connection lost";
        painter.setPen(QColor(Qt::red));
        painter.setFont(QFont("Arial", 20));
        painter.drawText(QRect(195, 430, 250, 100), Qt::AlignCenter, SpeedText);

        iconFont = QFont{"Material Icons", 50, QFont::Normal};
        painter.setFont(iconFont);
        painter.drawText(QRect(245, 350, 150, 150), Qt::AlignCenter, QChar(0xe628));
    }

    int circleRadius = 15;
    QPen circlePen;
    circlePen.setColor(Qt::white);
    circlePen.setWidth(4);
    circlePen.setStyle(Qt::SolidLine);
    painter.setPen(circlePen);
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(centerX - circleRadius, centerY - circleRadius, circleRadius * 2, circleRadius * 2);

    // Draw the speedometer needle

    // Draw integer labels and markings on speedometer

    QFont materialIconsFont("Material Icons", 12);
    painter.setFont(materialIconsFont);

    int labelRadius = radius - 70;
    int labelPadding = 8;
    int tickLength = 25;
    int tickPenWidth = 7;
    int tickstartAngle = -30 * 16;
    int tickspanAngle = 240 * 16;

    int tickTextPadding = 35;

    for (int tick = 0; tick <= 12; tick++) // 12 ticks for 12 labels
    {
        int speed = tick * 20;
        qreal angle = tickstartAngle + (tickspanAngle * (1.0 - (speed / 240.0)));
        qreal needleAngle = qDegreesToRadians(angle / 16.0);

        // Calculate label positions
        qreal labelX = centerX + (labelRadius + labelPadding) * qCos(needleAngle);
        qreal labelY = centerY - (labelRadius + labelPadding) * qSin(needleAngle);

        // Draw label
        QString label = QString::number(speed);
        QFontMetrics fm(painter.font());
        qreal textWidth = fm.horizontalAdvance(label); // Get the width of the text
        qreal textHeight = fm.height();                // Get the height of the text
        painter.setFont(QFont("Arial", 18, QFont::Bold));
        painter.setPen(QPen(Qt::white));
        painter.drawText(labelX - textWidth / 2, labelY + textHeight / 4, label); // Center the text

        // Draw tick marks
        qreal tickStartX = centerX + (radius - labelPadding) * qCos(needleAngle);
        qreal tickStartY = centerY - (radius - labelPadding) * qSin(needleAngle);
        qreal tickEndX = centerX + (radius - labelPadding - tickLength) * qCos(needleAngle);
        qreal tickEndY = centerY - (radius - labelPadding - tickLength) * qSin(needleAngle);
        painter.setPen(QPen(Qt::white, tickPenWidth, Qt::SolidLine, Qt::FlatCap));
        painter.drawLine(tickStartX, tickStartY, tickEndX, tickEndY);

        qreal tickIntervalAngle = tickspanAngle / (12 * 4); // Dividing by 4 for smaller ticks
        for (int smallTick = 1; smallTick <= 3; smallTick++)
        {
            qreal smallTickAngle = angle - smallTick * tickIntervalAngle;
            if (smallTickAngle >= tickstartAngle && smallTickAngle <= tickspanAngle) // Check if within 0 to 240 degrees
            {
                qreal smallTickStartX = centerX + (radius - labelPadding) * qCos(qDegreesToRadians(smallTickAngle / 16.0));
                qreal smallTickStartY = centerY - (radius - labelPadding) * qSin(qDegreesToRadians(smallTickAngle / 16.0));

                qreal smallTickEndX;
                qreal smallTickEndY;

                if (smallTick == 2) // Modify the tick length for the second tick
                {

                    qreal tickLengthModifier = 0.8; // Increase the tick length for the second tick
                    smallTickEndX = centerX + (radius - labelPadding - tickLength * tickLengthModifier) * qCos(qDegreesToRadians(smallTickAngle / 16.0));
                    smallTickEndY = centerY - (radius - labelPadding - tickLength * tickLengthModifier) * qSin(qDegreesToRadians(smallTickAngle / 16.0));
                    painter.setPen(QPen(Qt::white, tickPenWidth / 1.5, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLine(smallTickStartX, smallTickStartY, smallTickEndX, smallTickEndY);
                }
                else
                {
                    qreal tickLengthModifier = 0.5; // Reduce the tick length for other ticks
                    smallTickEndX = centerX + (radius - labelPadding - tickLength * tickLengthModifier) * qCos(qDegreesToRadians(smallTickAngle / 16.0));
                    smallTickEndY = centerY - (radius - labelPadding - tickLength * tickLengthModifier) * qSin(qDegreesToRadians(smallTickAngle / 16.0));
                    painter.setPen(QPen(Qt::white, tickPenWidth / 2, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLine(smallTickStartX, smallTickStartY, smallTickEndX, smallTickEndY);
                }
            }
        }
    }

    int needleLength = radius - 45; // Convert speed to qreal
    qreal needleAngle = 210 - speed;

    qreal needleX = centerX + needleLength * qCos(qDegreesToRadians(needleAngle));
    qreal needleY = centerY - needleLength * qSin(qDegreesToRadians(needleAngle));
    painter.setPen(QPen(Qt::red, 5));
    painter.drawLine(centerX, centerY, needleX, needleY);
}

void Canvas::fontBatteryLevel(void)
{
    painter.setPen(QColor(255, 255, 255));
    painter.setFont(iconText);
    painter.drawText(QRect(665, 340, 100, 100), Qt::AlignCenter, (QString::number(batteryLevel) + " %"));

    // Draw battery icon
    iconFont.setPixelSize(164);

    if (batteryLevel < 25)
    {

        painter.setPen(QColor(Qt::red));
    }
    else if (batteryLevel >= 25 && batteryLevel <= 49)
    {

        painter.setPen(QColor(Qt::yellow));
    }
    else if (batteryLevel >= 50)
    {

        painter.setPen(QColor(Qt::green));
    }
    painter.setFont(iconFont);
    painter.drawText(QRect(690, 230, 51, 164), Qt::AlignCenter, QChar(0xebdc));

    // batery icon level
    QRect squareRect(695, 370, 43, -batteryLevel);

    if (batteryLevel < 25)
    {

        painter.fillRect(squareRect, Qt::red);
    }
    else if (batteryLevel >= 25 && batteryLevel <= 49)
    {

        painter.fillRect(squareRect, Qt::yellow);
    }
    else if (batteryLevel >= 50)
    {

        painter.fillRect(squareRect, Qt::green);
    }
}

void Canvas::fontBlinkers(void)
{
    constexpr int period = 14;
    static uint8_t counter{0};
    static bool isBlinkerSoundPlaying = false;

    counter++;
    if (counter >= period)
    {
        counter = 0;
    }

    if (counter < (period / 2))
    {

        if (leftlight || rightlight)
        {
            painter.setPen(QColor(0, 255, 0));
            painter.setFont(QFont{"Material Icons", 46, QFont::Normal});
            if (!isBlinkerSoundPlaying)
            {
                blinkersound.play();
                isBlinkerSoundPlaying = true;
            }

            if (leftlight)
            {
                painter.drawText(QRect(10, 5, 40, 40), Qt::AlignCenter, QChar(0xe5c4));
            }

            if (rightlight)
            {
                painter.drawText(QRect(620, 5, 40, 40), Qt::AlignCenter, QChar(0xe5c8));
            }
        }
        else
        {
            // Stop the blinker sound when the blinkers are turned off
            if (isBlinkerSoundPlaying)
            {
                blinkersound.stop();
                isBlinkerSoundPlaying = false;
            }
        }
    }
}

void Canvas::fontTmpLevel(void)
{
    painter.setPen(QColor(255, 255, 255));
    painter.setFont(QFont("Arial", 13));
    painter.drawText(QRect(665, 425, 100, 100), Qt::AlignCenter, (QString::number(tempreature) + " °C"));

    // Draw temperature icon
    iconFont = QFont{"Material Icons", 35, QFont::Normal};
    if (tempreature < 5)
    {

        painter.setPen(QColor(Qt::white));
    }
    else if (tempreature >= 5 && tempreature <= 39)
    {

        painter.setPen(QColor(Qt::blue));
    }
    else if (tempreature > 39)
    {

        painter.setPen(QColor(Qt::red));
    }
    painter.setFont(iconFont);
    iconFont.setPixelSize(40);
    painter.drawText(QRect(665, 390, 100, 100), Qt::AlignCenter, QChar(0xf076));
}
