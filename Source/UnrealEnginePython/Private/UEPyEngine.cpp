#include "UnrealEnginePythonPrivatePCH.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"



PyObject *py_unreal_engine_log(PyObject * self, PyObject * args) {
	PyObject *py_message;
	if (!PyArg_ParseTuple(args, "O:log", &py_message)) {
		return NULL;
	}

	PyObject *stringified = PyObject_Str(py_message);
	if (!stringified)
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	char *message = PyUnicode_AsUTF8(stringified);
	UE_LOG(LogPython, Log, TEXT("%s"), UTF8_TO_TCHAR(message));
	Py_DECREF(stringified);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_unreal_engine_log_warning(PyObject * self, PyObject * args) {
	PyObject *py_message;
	if (!PyArg_ParseTuple(args, "O:log_warning", &py_message)) {
		return NULL;
	}

	PyObject *stringified = PyObject_Str(py_message);
	if (!stringified)
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	char *message = PyUnicode_AsUTF8(stringified);
	UE_LOG(LogPython, Warning, TEXT("%s"), UTF8_TO_TCHAR(message));
	Py_DECREF(stringified);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_unreal_engine_log_error(PyObject * self, PyObject * args) {
	PyObject *py_message;
	if (!PyArg_ParseTuple(args, "O:log_error", &py_message)) {
		return NULL;
	}

	PyObject *stringified = PyObject_Str(py_message);
	if (!stringified)
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	char *message = PyUnicode_AsUTF8(stringified);
	UE_LOG(LogPython, Error, TEXT("%s"), UTF8_TO_TCHAR(message));
	Py_DECREF(stringified);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_unreal_engine_add_on_screen_debug_message(PyObject * self, PyObject * args) {
	int key;
	float time_to_display;
	PyObject *py_message;
	if (!PyArg_ParseTuple(args, "ifO:add_on_screen_debug_message", &key, &time_to_display, &py_message)) {
		return NULL;
	}

	if (!GEngine) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject *stringified = PyObject_Str(py_message);
	if (!stringified)
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	char *message = PyUnicode_AsUTF8(stringified);

	GEngine->AddOnScreenDebugMessage(key, time_to_display, FColor::Green, FString::Printf(TEXT("%s"), UTF8_TO_TCHAR(message)));

	Py_DECREF(stringified);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_unreal_engine_print_string(PyObject * self, PyObject * args) {

	PyObject *py_message;
	float timeout = 2.0;
	PyObject *py_color = nullptr;
	if (!PyArg_ParseTuple(args, "O|fO:print_string", &py_message, &timeout, &py_color)) {
		return NULL;
	}

	if (!GEngine) {
		Py_RETURN_NONE;
	}

	FColor color = FColor::Cyan;

	if (py_color) {
		ue_PyFColor *f_color = py_ue_is_fcolor(py_color);
		if (!f_color)
			return PyErr_Format(PyExc_Exception, "argument is not a FColor");
		color = f_color->color;
	}

	PyObject *stringified = PyObject_Str(py_message);
	if (!stringified)
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	char *message = PyUnicode_AsUTF8(stringified);

	GEngine->AddOnScreenDebugMessage(-1, timeout, color, FString(UTF8_TO_TCHAR(message)));

	Py_DECREF(stringified);

	Py_RETURN_NONE;
}

PyObject *py_unreal_engine_get_forward_vector(PyObject * self, PyObject * args) {
	FRotator rot;
	if (!py_ue_rotator_arg(args, rot))
		return NULL;
	FVector vec = UKismetMathLibrary::GetForwardVector(rot);
	return py_ue_new_fvector(vec);
}

PyObject *py_unreal_engine_get_right_vector(PyObject * self, PyObject * args) {
	FRotator rot;
	if (!py_ue_rotator_arg(args, rot))
		return NULL;
	FVector vec = UKismetMathLibrary::GetRightVector(rot);
	return py_ue_new_fvector(vec);
}

PyObject *py_unreal_engine_get_up_vector(PyObject * self, PyObject * args) {
	FRotator rot;
	if (!py_ue_rotator_arg(args, rot))
		return NULL;
	FVector vec = UKismetMathLibrary::GetUpVector(rot);
	return py_ue_new_fvector(vec);
}

PyObject *py_unreal_engine_get_content_dir(PyObject * self, PyObject * args) {
	return PyUnicode_FromString(TCHAR_TO_UTF8(*FPaths::GameContentDir()));
}

PyObject *py_unreal_engine_convert_relative_path_to_full(PyObject * self, PyObject * args) {
	char *path;
	if (!PyArg_ParseTuple(args, "s:convert_relative_path_to_full", &path)) {
		return NULL;
	}
	return PyUnicode_FromString(TCHAR_TO_UTF8(*FPaths::ConvertRelativePathToFull(UTF8_TO_TCHAR(path))));
}

PyObject *py_unreal_engine_object_path_to_package_name(PyObject * self, PyObject * args) {
	char *path;
	if (!PyArg_ParseTuple(args, "s:object_path_to_package_name", &path)) {
		return NULL;
	}
	return PyUnicode_FromString(TCHAR_TO_UTF8(*FPackageName::ObjectPathToPackageName(UTF8_TO_TCHAR(path))));
}

PyObject *py_unreal_engine_get_path(PyObject * self, PyObject * args) {
	char *path;
	if (!PyArg_ParseTuple(args, "s:get_path", &path)) {
		return NULL;
	}
	return PyUnicode_FromString(TCHAR_TO_UTF8(*FPaths::GetPath(UTF8_TO_TCHAR(path))));
}

PyObject *py_unreal_engine_get_base_filename(PyObject * self, PyObject * args) {
	char *path;
	if (!PyArg_ParseTuple(args, "s:get_base_filename", &path)) {
		return NULL;
	}
	return PyUnicode_FromString(TCHAR_TO_UTF8(*FPaths::GetBaseFilename(UTF8_TO_TCHAR(path))));
}

PyObject *py_unreal_engine_create_world(PyObject * self, PyObject * args) {
	int world_type = 0;
	if (!PyArg_ParseTuple(args, "|i:create_world", &world_type)) {
		return NULL;
	}

	UWorld *world = UWorld::CreateWorld((EWorldType::Type)world_type, false);

	ue_PyUObject *ret = ue_get_python_wrapper(world);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_find_class(PyObject * self, PyObject * args) {
	char *name;
	if (!PyArg_ParseTuple(args, "s:find_class", &name)) {
		return NULL;
	}

	UClass *u_class = FindObject<UClass>(ANY_PACKAGE, UTF8_TO_TCHAR(name));

	if (!u_class)
		return PyErr_Format(PyExc_Exception, "unable to find class %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_class);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_find_enum(PyObject * self, PyObject * args) {
	char *name;
	if (!PyArg_ParseTuple(args, "s:find_enum", &name)) {
		return NULL;
	}

	UEnum *u_enum = FindObject<UEnum>(ANY_PACKAGE, UTF8_TO_TCHAR(name));

	if (!u_enum)
		return PyErr_Format(PyExc_Exception, "unable to find enum %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_enum);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;

}

PyObject *py_unreal_engine_load_package(PyObject * self, PyObject * args) {
	char *name;
	if (!PyArg_ParseTuple(args, "s:load_package", &name)) {
		return nullptr;
	}

	UPackage *u_package = LoadPackage(nullptr, UTF8_TO_TCHAR(name), LOAD_None);

	if (!u_package)
		return PyErr_Format(PyExc_Exception, "unable to load package %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_package);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_load_class(PyObject * self, PyObject * args) {
	char *name;
	char *filename = nullptr;
	if (!PyArg_ParseTuple(args, "s|s:load_class", &name, &filename)) {
		return NULL;
	}

	TCHAR *t_filename = (TCHAR *)0;
	if (filename)
		t_filename = UTF8_TO_TCHAR(filename);

	UObject *u_class = StaticLoadObject(UClass::StaticClass(), NULL, UTF8_TO_TCHAR(name), t_filename);

	if (!u_class)
		return PyErr_Format(PyExc_Exception, "unable to find class %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_class);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_load_enum(PyObject * self, PyObject * args) {
	char *name;
	char *filename = nullptr;
	if (!PyArg_ParseTuple(args, "s|s:load_enum", &name, &filename)) {
		return NULL;
	}

	TCHAR *t_filename = (TCHAR *)0;
	if (filename)
		t_filename = UTF8_TO_TCHAR(filename);

	UObject *u_enum = StaticLoadObject(UEnum::StaticClass(), NULL, UTF8_TO_TCHAR(name), t_filename);

	if (!u_enum)
		return PyErr_Format(PyExc_Exception, "unable to find enum %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_enum);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_find_struct(PyObject * self, PyObject * args) {
	char *name;
	if (!PyArg_ParseTuple(args, "s:find_struct", &name)) {
		return NULL;
	}

	UScriptStruct *u_struct = FindObject<UScriptStruct>(ANY_PACKAGE, UTF8_TO_TCHAR(name));

	if (!u_struct)
		return PyErr_Format(PyExc_Exception, "unable to find struct %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_struct);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;

}

PyObject *py_unreal_engine_load_struct(PyObject * self, PyObject * args) {
	char *name;
	char *filename = nullptr;
	if (!PyArg_ParseTuple(args, "s|s:load_struct", &name, &filename)) {
		return NULL;
	}

	TCHAR *t_filename = (TCHAR *)0;
	if (filename)
		t_filename = UTF8_TO_TCHAR(filename);

	UObject *u_struct = StaticLoadObject(UScriptStruct::StaticClass(), NULL, UTF8_TO_TCHAR(name), t_filename);

	if (!u_struct)
		return PyErr_Format(PyExc_Exception, "unable to find struct %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_struct);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;

}


PyObject *py_unreal_engine_load_object(PyObject * self, PyObject * args) {
	PyObject *obj;
	char *name;
	char *filename = nullptr;
	if (!PyArg_ParseTuple(args, "Os|s:load_object", &obj, &name, &filename)) {
		return NULL;
	}

	if (!ue_is_pyuobject(obj)) {
		return PyErr_Format(PyExc_Exception, "argument is not a UObject");
	}

	ue_PyUObject *py_obj = (ue_PyUObject *)obj;
	if (!py_obj->ue_object->IsA<UClass>()) {
		return PyErr_Format(PyExc_Exception, "argument is not a UClass");
	}

	UClass *u_class = (UClass *)py_obj->ue_object;

	TCHAR *t_filename = (TCHAR *)0;
	if (filename)
		t_filename = UTF8_TO_TCHAR(filename);

	UObject *u_object = StaticLoadObject(u_class, NULL, UTF8_TO_TCHAR(name), t_filename);

	if (!u_object)
		return PyErr_Format(PyExc_Exception, "unable to load object %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_object);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;

}

PyObject *py_unreal_engine_string_to_guid(PyObject * self, PyObject * args) {
	char *str;
	if (!PyArg_ParseTuple(args, "s:string_to_guid", &str)) {
		return NULL;
	}

	FGuid guid;

	if (FGuid::Parse(FString(str), guid)) {
		return py_ue_new_uscriptstruct(FindObject<UScriptStruct>(ANY_PACKAGE, UTF8_TO_TCHAR((char *)"Guid")), (uint8 *)&guid);
	}

	return PyErr_Format(PyExc_Exception, "unable to build FGuid");
}

PyObject *py_unreal_engine_slate_tick(PyObject * self, PyObject * args) {
	FSlateApplication::Get().PumpMessages();
	FSlateApplication::Get().Tick();
	Py_RETURN_NONE;
}

PyObject *py_unreal_engine_engine_tick(PyObject * self, PyObject * args) {
	float delta_seconds = FApp::GetDeltaTime();
	PyObject *py_bool = nullptr;
	if (!PyArg_ParseTuple(args, "|fO:engine_tick", &delta_seconds, &py_bool)) {
		return NULL;
	}

	GEngine->Tick(delta_seconds, (py_bool && PyObject_IsTrue(py_bool)) ? true : false);

	Py_RETURN_NONE;
}

PyObject *py_unreal_engine_get_delta_time(PyObject * self, PyObject * args) {
	return PyFloat_FromDouble(FApp::GetDeltaTime());
}


PyObject *py_unreal_engine_find_object(PyObject * self, PyObject * args) {
	char *name;
	if (!PyArg_ParseTuple(args, "s:find_object", &name)) {
		return NULL;
	}

	UObject *u_object = FindObject<UObject>(ANY_PACKAGE, UTF8_TO_TCHAR(name));

	if (!u_object)
		return PyErr_Format(PyExc_Exception, "unable to find object %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_object);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}


PyObject *py_unreal_engine_new_object(PyObject * self, PyObject * args) {

	PyObject *obj;
	PyObject *py_outer = NULL;
	char *name = nullptr;
	if (!PyArg_ParseTuple(args, "O|Os:new_object", &obj, &py_outer, &name)) {
		return NULL;
	}

	if (!ue_is_pyuobject(obj)) {
		return PyErr_Format(PyExc_Exception, "argument is not a UObject");
	}

	ue_PyUObject *py_obj = (ue_PyUObject *)obj;

	if (!py_obj->ue_object->IsA<UClass>())
		return PyErr_Format(PyExc_Exception, "uobject is not a UClass");

	UClass *obj_class = (UClass *)py_obj->ue_object;

	FName f_name = NAME_None;

	if (name && strlen(name) > 0) {
		f_name = FName(UTF8_TO_TCHAR(name));
	}

	UObject *outer = GetTransientPackage();

	if (py_outer && py_outer != Py_None) {
		if (!ue_is_pyuobject(py_outer)) {
			return PyErr_Format(PyExc_Exception, "argument is not a UObject");
		}

		ue_PyUObject *py_outer_obj = (ue_PyUObject *)py_outer;

		outer = py_outer_obj->ue_object;
	}

	UObject *new_object = NewObject<UObject>(outer, obj_class, f_name, RF_Public | RF_Standalone);
	if (!new_object)
		return PyErr_Format(PyExc_Exception, "unable to create object");

	new_object->PostLoad();

	ue_PyUObject *ret = ue_get_python_wrapper(new_object);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_get_mutable_default(PyObject * self, PyObject * args) {

	PyObject *obj;
	if (!PyArg_ParseTuple(args, "O|Os:new_object", &obj)) {
		return NULL;
	}

	if (!ue_is_pyuobject(obj)) {
		return PyErr_Format(PyExc_Exception, "argument is not a UObject");
	}

	ue_PyUObject *py_obj = (ue_PyUObject *)obj;

	if (!py_obj->ue_object->IsA<UClass>())
		return PyErr_Format(PyExc_Exception, "uobject is not a UClass");

	UClass *obj_class = (UClass *)py_obj->ue_object;

	UObject *mutable_object = GetMutableDefault<UObject>(obj_class);
	if (!mutable_object)
		return PyErr_Format(PyExc_Exception, "unable to create object");

	ue_PyUObject *ret = ue_get_python_wrapper(mutable_object);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}


PyObject *py_unreal_engine_new_class(PyObject * self, PyObject * args) {

	PyObject *py_parent;
	char *name;
	if (!PyArg_ParseTuple(args, "Os:new_class", &py_parent, &name)) {
		return NULL;
	}

	UClass *parent = nullptr;

	if (py_parent != Py_None) {
		if (!ue_is_pyuobject(py_parent)) {
			return PyErr_Format(PyExc_Exception, "argument is not a UObject");
		}
		ue_PyUObject *py_obj = (ue_PyUObject *)py_parent;
		if (!py_obj->ue_object->IsA<UClass>())
			return PyErr_Format(PyExc_Exception, "uobject is not a UClass");
		parent = (UClass *)py_obj->ue_object;
	}

	UClass *new_object = unreal_engine_new_uclass(name, parent);
	if (!new_object)
		return PyErr_Format(PyExc_Exception, "unable to create UClass");

	ue_PyUObject *ret = ue_get_python_wrapper(new_object);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_all_classes(PyObject * self, PyObject * args) {

	PyObject *ret = PyList_New(0);

	for (TObjectIterator<UClass> Itr; Itr; ++Itr) {
		ue_PyUObject *py_obj = ue_get_python_wrapper(*Itr);
		if (!py_obj)
			continue;
		PyList_Append(ret, (PyObject *)py_obj);
	}
	return ret;
}

PyObject *py_unreal_engine_create_and_dispatch_when_ready(PyObject * self, PyObject * args) {
	PyObject *py_callable;
	if (!PyArg_ParseTuple(args, "O:create_and_dispatch_when_ready", &py_callable)) {
		return NULL;
	}

	if (!PyCallable_Check(py_callable))
		return PyErr_Format(PyExc_TypeError, "argument is not callable");

	Py_INCREF(py_callable);

	FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]() {
		FScopePythonGIL gil;
		PyObject *ret = PyObject_CallObject(py_callable, nullptr);
		if (ret) {
			Py_DECREF(ret);
		}
		else {
			unreal_engine_py_log_error();
		}
		Py_DECREF(py_callable);
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
	// TODO Implement signal triggering in addition to WaitUntilTaskCompletes
	// FTaskGraphInterface::Get().TriggerEventWhenTaskCompletes

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_unreal_engine_get_game_viewport_size(PyObject *self, PyObject * args) {

	if (!GEngine->GameViewport) {
		return PyErr_Format(PyExc_Exception, "unable to get GameViewport");
	}

	FVector2D size;
	GEngine->GameViewport->GetViewportSize(size);

	return Py_BuildValue("(ff)", size.X, size.Y);
}


PyObject *py_unreal_engine_get_resolution(PyObject *self, PyObject * args) {
	return Py_BuildValue("(ff)", GSystemResolution.ResX, GSystemResolution.ResY);
}

PyObject *py_unreal_engine_get_viewport_screenshot(PyObject *self, PyObject * args) {

	if (!GEngine->GameViewport) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject *py_bool = nullptr;
	bool as_int_list = false;
	if (!PyArg_ParseTuple(args, "|O:get_viewport_screenshot", &py_bool)) {
		return NULL;
	}

	if (py_bool && PyObject_IsTrue(py_bool))
		as_int_list = true;

	FViewport *viewport = GEngine->GameViewport->Viewport;
	TArray<FColor> bitmap;

	bool success = GetViewportScreenShot(viewport, bitmap);

	if (!success) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	if (as_int_list) {
		PyObject *bitmap_tuple = PyTuple_New(bitmap.Num() * 4);
		for (int i = 0; i < bitmap.Num(); i++) {
			PyTuple_SetItem(bitmap_tuple, i * 4, PyLong_FromLong(bitmap[i].R));
			PyTuple_SetItem(bitmap_tuple, i * 4 + 1, PyLong_FromLong(bitmap[i].G));
			PyTuple_SetItem(bitmap_tuple, i * 4 + 2, PyLong_FromLong(bitmap[i].B));
			PyTuple_SetItem(bitmap_tuple, i * 4 + 3, PyLong_FromLong(bitmap[i].A));
		}
		return bitmap_tuple;
	}

	PyObject *bitmap_tuple = PyTuple_New(bitmap.Num());
	for (int i = 0; i < bitmap.Num(); i++) {
		PyTuple_SetItem(bitmap_tuple, i, py_ue_new_fcolor(bitmap[i]));
	}

	return bitmap_tuple;
}

PyObject *py_unreal_engine_get_viewport_size(PyObject *self, PyObject * args) {

	if (!GEngine->GameViewport) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	FViewport *viewport = GEngine->GameViewport->Viewport;
	PyObject *tuple_size = PyTuple_New(2);
	FIntPoint point = viewport->GetSizeXY();

	PyTuple_SetItem(tuple_size, 0, PyLong_FromLong(point.X));
	PyTuple_SetItem(tuple_size, 1, PyLong_FromLong(point.Y));

	return tuple_size;
}

#if WITH_EDITOR
PyObject *py_unreal_engine_editor_get_active_viewport_screenshot(PyObject *self, PyObject * args) {

	FViewport *viewport = GEditor->GetActiveViewport();
	if (!viewport) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject *py_bool = nullptr;
	bool as_int_list = false;
	if (!PyArg_ParseTuple(args, "|O:editor_get_active_viewport_screenshot", &py_bool)) {
		return NULL;
	}

	if (py_bool && PyObject_IsTrue(py_bool))
		as_int_list = true;

	TArray<FColor> bitmap;

	bool success = GetViewportScreenShot(viewport, bitmap);

	if (!success) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	if (as_int_list) {
		PyObject *bitmap_tuple = PyTuple_New(bitmap.Num() * 4);
		for (int i = 0; i < bitmap.Num(); i++) {
			PyTuple_SetItem(bitmap_tuple, i * 4, PyLong_FromLong(bitmap[i].R));
			PyTuple_SetItem(bitmap_tuple, i * 4 + 1, PyLong_FromLong(bitmap[i].G));
			PyTuple_SetItem(bitmap_tuple, i * 4 + 2, PyLong_FromLong(bitmap[i].B));
			PyTuple_SetItem(bitmap_tuple, i * 4 + 3, PyLong_FromLong(bitmap[i].A));
		}
		return bitmap_tuple;
	}

	PyObject *bitmap_tuple = PyTuple_New(bitmap.Num());
	for (int i = 0; i < bitmap.Num(); i++) {
		PyTuple_SetItem(bitmap_tuple, i, py_ue_new_fcolor(bitmap[i]));
	}

	return bitmap_tuple;
}

PyObject *py_unreal_engine_editor_get_active_viewport_size(PyObject *self, PyObject * args) {


	FViewport *viewport = GEditor->GetActiveViewport();
	if (!viewport) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject *tuple_size = PyTuple_New(2);

	FIntPoint point = viewport->GetSizeXY();

	PyTuple_SetItem(tuple_size, 0, PyLong_FromLong(point.X));
	PyTuple_SetItem(tuple_size, 1, PyLong_FromLong(point.Y));

	return tuple_size;
}

PyObject *py_unreal_engine_editor_get_pie_viewport_screenshot(PyObject *self, PyObject * args) {

	FViewport *viewport = GEditor->GetPIEViewport();
	if (!viewport) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject *py_bool = nullptr;
	bool as_int_list = false;
	if (!PyArg_ParseTuple(args, "|O:editor_get_pie_viewport_screenshot", &py_bool)) {
		return NULL;
	}

	if (py_bool && PyObject_IsTrue(py_bool))
		as_int_list = true;

	TArray<FColor> bitmap;

	bool success = GetViewportScreenShot(viewport, bitmap);

	if (!success) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	if (as_int_list) {
		PyObject *bitmap_tuple = PyTuple_New(bitmap.Num() * 4);
		for (int i = 0; i < bitmap.Num(); i++) {
			PyTuple_SetItem(bitmap_tuple, i * 4, PyLong_FromLong(bitmap[i].R));
			PyTuple_SetItem(bitmap_tuple, i * 4 + 1, PyLong_FromLong(bitmap[i].G));
			PyTuple_SetItem(bitmap_tuple, i * 4 + 2, PyLong_FromLong(bitmap[i].B));
			PyTuple_SetItem(bitmap_tuple, i * 4 + 3, PyLong_FromLong(bitmap[i].A));
		}
		return bitmap_tuple;
	}

	PyObject *bitmap_tuple = PyTuple_New(bitmap.Num());
	for (int i = 0; i < bitmap.Num(); i++) {
		PyTuple_SetItem(bitmap_tuple, i, py_ue_new_fcolor(bitmap[i]));
	}

	return bitmap_tuple;
}

PyObject *py_unreal_engine_editor_get_pie_viewport_size(PyObject *self, PyObject * args) {


	FViewport *viewport = GEditor->GetPIEViewport();
	if (!viewport) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject *tuple_size = PyTuple_New(2);
	FIntPoint point = viewport->GetSizeXY();

	PyTuple_SetItem(tuple_size, 0, PyLong_FromLong(point.X));
	PyTuple_SetItem(tuple_size, 1, PyLong_FromLong(point.Y));
	return tuple_size;
}
#endif

PyObject *py_unreal_engine_create_package(PyObject *self, PyObject * args) {

	char *name;

	if (!PyArg_ParseTuple(args, "s:create_package", &name)) {
		return nullptr;
	}

	UPackage *u_package = (UPackage *)StaticFindObject(nullptr, ANY_PACKAGE, UTF8_TO_TCHAR(name), true);
	// create a new package if it does not exist
	if (u_package) {
		return PyErr_Format(PyExc_Exception, "package %s already exists", TCHAR_TO_UTF8(*u_package->GetPathName()));
	}
	u_package = CreatePackage(nullptr, UTF8_TO_TCHAR(name));
	if (!u_package)
		return PyErr_Format(PyExc_Exception, "unable to create package");
	u_package->FileName = *FPackageName::LongPackageNameToFilename(UTF8_TO_TCHAR(name), FPackageName::GetAssetPackageExtension());

	u_package->FullyLoad();
	u_package->MarkPackageDirty();

	ue_PyUObject *ret = ue_get_python_wrapper(u_package);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_get_or_create_package(PyObject *self, PyObject * args) {

	char *name;

	if (!PyArg_ParseTuple(args, "s:get_or_create_package", &name)) {
		return nullptr;
	}

	UPackage *u_package = (UPackage *)StaticFindObject(nullptr, ANY_PACKAGE, UTF8_TO_TCHAR(name), true);
	// create a new package if it does not exist
	if (!u_package) {
		u_package = CreatePackage(nullptr, UTF8_TO_TCHAR(name));
		if (!u_package)
			return PyErr_Format(PyExc_Exception, "unable to create package");
		u_package->FileName = *FPackageName::LongPackageNameToFilename(UTF8_TO_TCHAR(name), FPackageName::GetAssetPackageExtension());

		u_package->FullyLoad();
		u_package->MarkPackageDirty();
	}

	ue_PyUObject *ret = ue_get_python_wrapper(u_package);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_get_transient_package(PyObject *self, PyObject * args) {

	ue_PyUObject *ret = ue_get_python_wrapper(GetTransientPackage());
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_open_file_dialog(PyObject *self, PyObject * args) {
	char *title;
	char *default_path = (char *)"";
	char *default_file = (char *)"";
	char *file_types = (char *)"";
	PyObject *py_multiple = nullptr;

	if (!PyArg_ParseTuple(args, "s|sssO:open_file_dialog", &title, &default_path, &default_file, &file_types, &py_multiple))
		return nullptr;

	IDesktopPlatform *DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
		return PyErr_Format(PyExc_Exception, "unable to get reference to DesktopPlatform module");

	TSharedPtr<SWindow> ParentWindow = FGlobalTabmanager::Get()->GetRootWindow();
	if (!ParentWindow.IsValid())
		return PyErr_Format(PyExc_Exception, "unable to get the Root Window");

	if (!ParentWindow->GetNativeWindow().IsValid())
		return PyErr_Format(PyExc_Exception, "unable to get Native Root Window");

	TArray<FString> files;

	if (!DesktopPlatform->OpenFileDialog(ParentWindow->GetNativeWindow()->GetOSWindowHandle(), FString(UTF8_TO_TCHAR(title)),
		FString(UTF8_TO_TCHAR(default_path)),
		FString(UTF8_TO_TCHAR(default_file)),
		FString(UTF8_TO_TCHAR(file_types)),
		(py_multiple && PyObject_IsTrue(py_multiple)) ? EFileDialogFlags::Multiple : EFileDialogFlags::None,
		files)) {
		Py_RETURN_NONE;
	}

	PyObject *py_list = PyList_New(0);
	for (FString file : files) {
		PyList_Append(py_list, PyUnicode_FromString(TCHAR_TO_UTF8(*file)));
	}
	return py_list;
}

PyObject *py_unreal_engine_open_directory_dialog(PyObject *self, PyObject * args) {
	char *title;
	char *default_path = (char *)"";

	if (!PyArg_ParseTuple(args, "s|s:open_directory_dialog", &title, &default_path))
		return nullptr;

	IDesktopPlatform *DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
		return PyErr_Format(PyExc_Exception, "unable to get reference to DesktopPlatform module");

	TSharedPtr<SWindow> ParentWindow = FGlobalTabmanager::Get()->GetRootWindow();
	if (!ParentWindow.IsValid())
		return PyErr_Format(PyExc_Exception, "unable to get the Root Window");

	if (!ParentWindow->GetNativeWindow().IsValid())
		return PyErr_Format(PyExc_Exception, "unable to get Native Root Window");

	FString choosen_dir;

	if (!DesktopPlatform->OpenDirectoryDialog(ParentWindow->GetNativeWindow()->GetOSWindowHandle(), FString(UTF8_TO_TCHAR(title)),
		FString(UTF8_TO_TCHAR(default_path)),
		choosen_dir)) {
		Py_RETURN_NONE;
	}

	return PyUnicode_FromString(TCHAR_TO_UTF8(*choosen_dir));
}

PyObject *py_unreal_engine_open_font_dialog(PyObject *self, PyObject * args) {
	
	IDesktopPlatform *DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
		return PyErr_Format(PyExc_Exception, "unable to get reference to DesktopPlatform module");

	TSharedPtr<SWindow> ParentWindow = FGlobalTabmanager::Get()->GetRootWindow();
	if (!ParentWindow.IsValid())
		return PyErr_Format(PyExc_Exception, "unable to get the Root Window");

	if (!ParentWindow->GetNativeWindow().IsValid())
		return PyErr_Format(PyExc_Exception, "unable to get Native Root Window");

	FString font_name;
	float height;
	EFontImportFlags flags;

	if (!DesktopPlatform->OpenFontDialog(ParentWindow->GetNativeWindow()->GetOSWindowHandle(), font_name, height, flags)) {
		Py_RETURN_NONE;
	}

	return Py_BuildValue((char*)"(sfi)", TCHAR_TO_UTF8(*font_name), height, flags);
}

PyObject *py_unreal_engine_save_file_dialog(PyObject *self, PyObject * args) {
	char *title;
	char *default_path = (char *)"";
	char *default_file = (char *)"";
	char *file_types = (char *)"";
	PyObject *py_multiple = nullptr;

	if (!PyArg_ParseTuple(args, "s|sssO:save_file_dialog", &title, &default_path, &default_file, &file_types, &py_multiple))
		return nullptr;

	IDesktopPlatform *DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
		return PyErr_Format(PyExc_Exception, "unable to get reference to DesktopPlatform module");

	TSharedPtr<SWindow> ParentWindow = FGlobalTabmanager::Get()->GetRootWindow();
	if (!ParentWindow.IsValid())
		return PyErr_Format(PyExc_Exception, "unable to get the Root Window");

	if (!ParentWindow->GetNativeWindow().IsValid())
		return PyErr_Format(PyExc_Exception, "unable to get Native Root Window");

	TArray<FString> files;

	if (!DesktopPlatform->SaveFileDialog(ParentWindow->GetNativeWindow()->GetOSWindowHandle(), FString(UTF8_TO_TCHAR(title)),
		FString(UTF8_TO_TCHAR(default_path)),
		FString(UTF8_TO_TCHAR(default_file)),
		FString(UTF8_TO_TCHAR(file_types)),
		(py_multiple && PyObject_IsTrue(py_multiple)) ? EFileDialogFlags::Multiple : EFileDialogFlags::None,
		files)) {
		Py_RETURN_NONE;
	}

	PyObject *py_list = PyList_New(0);
	for (FString file : files) {
		PyList_Append(py_list, PyUnicode_FromString(TCHAR_TO_UTF8(*file)));
	}
	return py_list;
}