#include "input.h"

namespace NeovimQt { namespace Input {

Qt::KeyboardModifiers ControlModifier() noexcept
{
	return Qt::MetaModifier;
}

Qt::KeyboardModifiers CmdModifier() noexcept
{
	return Qt::ControlModifier;
}

Qt::Key Key_Control() noexcept
{
	return Qt::Key_Meta;
}

Qt::Key Key_Cmd() noexcept
{
	return Qt::Key_Control;
}

QString GetModifierPrefix(Qt::KeyboardModifiers mod) noexcept
{
	QString modprefix;
	if (mod & CmdModifier()) {
		// This behavior is from MacVim.
		modprefix += "D-";
	}
	if (mod & ControlModifier()) {
		modprefix += "C-";
	}
	if (mod & Qt::ShiftModifier) {
		modprefix += "S-";
	}
	if (mod & Qt::AltModifier) {
		modprefix += "A-";
	}

	return modprefix;
}

QString GetTextFromQKeyEvent(QKeyEvent *ev) noexcept
{
	const QChar c = QChar(ev->key());

  if (ev->key() == Qt::Key_BracketLeft && ev->modifiers() & Qt::ShiftModifier) {
    return "{";
  }

  if (ev->key() == Qt::Key_BracketRight && ev->modifiers() & Qt::ShiftModifier) {
    return "}";
  }

	if (c.isPrint()) {
		if (!(ev->modifiers() & Qt::KeyboardModifier::ShiftModifier)) {
			return c.toLower();
		}

		return c;
	}

	return "";
}

} } // namespace NeovimQt::Input
