#pragma once
#include "macro.hpp"
#include "scene-selection.hpp"

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>

enum class SceneType {
	CURRENT,
	PREVIOUS,
	CHANGED,
	NOTCHANGED,
};

class MacroConditionScene : public MacroCondition {
public:
	MacroConditionScene(Macro *m) : MacroCondition(m) {}
	bool CheckCondition();
	bool Save(obs_data_t *obj);
	bool Load(obs_data_t *obj);
	std::string GetShortDesc();
	std::string GetId() { return id; };
	static std::shared_ptr<MacroCondition> Create(Macro *m)
	{
		return std::make_shared<MacroConditionScene>(m);
	}

	SceneSelection _scene;
	SceneType _type = SceneType::CURRENT;
	// During a transition "current" scene could either stand for the scene
	// being transitioned to or the scene still being transitioned away
	// from.
	bool _useTransitionTargetScene = false;

private:
	std::chrono::high_resolution_clock::time_point _lastSceneChangeTime{};
	static bool _registered;
	static const std::string id;
};

class MacroConditionSceneEdit : public QWidget {
	Q_OBJECT

public:
	MacroConditionSceneEdit(
		QWidget *parent,
		std::shared_ptr<MacroConditionScene> cond = nullptr);
	void UpdateEntryData();
	static QWidget *Create(QWidget *parent,
			       std::shared_ptr<MacroCondition> cond)
	{
		return new MacroConditionSceneEdit(
			parent,
			std::dynamic_pointer_cast<MacroConditionScene>(cond));
	}

private slots:
	void SceneChanged(const SceneSelection &);
	void TypeChanged(int value);
	void UseTransitionTargetSceneChanged(int state);
signals:
	void HeaderInfoChanged(const QString &);

protected:
	SceneSelectionWidget *_scenes;
	QComboBox *_sceneType;
	QCheckBox *_useTransitionTargetScene;
	std::shared_ptr<MacroConditionScene> _entryData;

private:
	void SetWidgetVisibility();
	bool _loading = true;
};
