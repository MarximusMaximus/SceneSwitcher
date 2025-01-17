#pragma once
#include "macro.hpp"

#include <QComboBox>
#include <QCheckBox>

class MacroConditionWindow : public MacroCondition {
public:
	MacroConditionWindow(Macro *m) : MacroCondition(m) {}
	bool CheckCondition();
	bool Save(obs_data_t *obj);
	bool Load(obs_data_t *obj);
	std::string GetShortDesc();
	std::string GetId() { return id; };
	static std::shared_ptr<MacroCondition> Create(Macro *m)
	{
		return std::make_shared<MacroConditionWindow>(m);
	}

private:
	bool CheckWindowTitleSwitchDirect(std::string &currentWindowTitle);
	bool CheckWindowTitleSwitchRegex(std::string &currentWindowTitle,
					 std::vector<std::string> &windowList);

public:
	std::string _window;
	bool _fullscreen = false;
	bool _maximized = false;
	bool _focus = true;
	bool _windowFocusChanged = false;

private:
	static bool _registered;
	static const std::string id;
};

class MacroConditionWindowEdit : public QWidget {
	Q_OBJECT

public:
	MacroConditionWindowEdit(
		QWidget *parent,
		std::shared_ptr<MacroConditionWindow> cond = nullptr);
	void UpdateEntryData();
	static QWidget *Create(QWidget *parent,
			       std::shared_ptr<MacroCondition> cond)
	{
		return new MacroConditionWindowEdit(
			parent,
			std::dynamic_pointer_cast<MacroConditionWindow>(cond));
	}

private slots:
	void WindowChanged(const QString &text);
	void FullscreenChanged(int state);
	void MaximizedChanged(int state);
	void FocusedChanged(int state);
	void WindowFocusChanged(int state);
signals:
	void HeaderInfoChanged(const QString &);

protected:
	QComboBox *_windowSelection;
	QCheckBox *_fullscreen;
	QCheckBox *_maximized;
	QCheckBox *_focused;
	QCheckBox *_windowFocusChanged;
	std::shared_ptr<MacroConditionWindow> _entryData;

private:
	bool _loading = true;
};
