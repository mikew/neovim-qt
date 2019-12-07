#include <gui/input.h>
#include <QKeyEvent>
#include <QtTest/QtTest>

class TestInput: public QObject
{
	Q_OBJECT

private slots:
	void TestSpecialKeys();
	void TestModifierKeys();
	void OnlyModifierKeysIgnored();
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

void TestInput::OnlyModifierKeysIgnored()
{
	const QList<Qt::Key> modifierKeyList {
		Qt::Key::Key_Super_L,
		Qt::Key::Key_Super_R,
		Qt::Key::Key_Control,
		Qt::Key::Key_Meta,
		Qt::Key::Key_Alt,
		Qt::Key::Key_Shift,
	};

	const QList<Qt::KeyboardModifier> keyboardModifierList {
		Qt::KeyboardModifier::NoModifier,
		Qt::KeyboardModifier::ShiftModifier,
		Qt::KeyboardModifier::ControlModifier,
		Qt::KeyboardModifier::AltModifier,
		Qt::KeyboardModifier::MetaModifier,
	};

	for (const auto& key : modifierKeyList) {
		for (const auto& mod : keyboardModifierList) {
			QKeyEvent keyEvent{ QEvent::KeyPress, key, mod };
			QCOMPARE(NeovimQt::Input::convertKey(keyEvent.text(), keyEvent.key(), keyEvent.modifiers()),
				QString{});
		}
	}

	// Issue#344: <C-S-> appears as <C-Space>
	QKeyEvent evIssue344{ QEvent::KeyPress, Qt::Key::Key_Control, Qt::KeyboardModifier::ShiftModifier};
	QCOMPARE(NeovimQt::Input::convertKey(evIssue344.text(), evIssue344.key(), evIssue344.modifiers()),
		QString{});

	// Issue#593: Pressing Control + Super inserts ^S
	QKeyEvent evIssue593{ QEvent::KeyPress, Qt::Key::Key_Super_L, Qt::KeyboardModifier::ControlModifier};
	QCOMPARE(NeovimQt::Input::convertKey(evIssue593.text(), evIssue593.key(), evIssue593.modifiers()),
		QString{});
}

QTEST_MAIN(TestInput)
#include "tst_input_unix.moc"
