#include <gui/input.h>
#include <QKeyEvent>
#include <QtTest/QtTest>

class TestInput: public QObject
{
	Q_OBJECT

private slots:
	void TestSpecialKeys();
	void TestModifierKeys();
};

void TestInput::TestModifierKeys()
{
	QCOMPARE(NeovimQt::Input::convertKey({}, Qt::Key_Meta, Qt::MetaModifier), QString{});
	QCOMPARE(NeovimQt::Input::convertKey({}, Qt::Key_Control, Qt::ControlModifier), QString{});
	QCOMPARE(NeovimQt::Input::convertKey({}, Qt::Key_Alt, Qt::AltModifier), QString{});
}

void TestInput::TestSpecialKeys()
{
	const QMap<int, QString>& specialKeys { NeovimQt::Input::GetSpecialKeysMap() };

	for (const auto k : specialKeys.keys()) {
		QCOMPARE(NeovimQt::Input::convertKey({}, k, Qt::NoModifier),
			QString("<%1>").arg(specialKeys.value(k)));

		QCOMPARE(NeovimQt::Input::convertKey({}, k, Qt::ControlModifier),
			QString("<C-%1>").arg(specialKeys.value(k)));

		QCOMPARE(NeovimQt::Input::convertKey({}, k, Qt::AltModifier),
			QString("<A-%1>").arg(specialKeys.value(k)));

		// On Unix Meta is the Windows key, treated as D-.
		QCOMPARE(NeovimQt::Input::convertKey({}, k, Qt::MetaModifier),
			QString("<D-%1>").arg(specialKeys.value(k)));
	}
}

QTEST_MAIN(TestInput)
#include "tst_input_unix.moc"
