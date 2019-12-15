#include <Python.h>
#include "structmember.h"

typedef struct {
    PyObject_HEAD
    PyObject *obj;
    int state;
} Shoddy;


static PyObject *
Shoddy_increment(Shoddy *self, PyObject *unused) {
    self->state++;
    return PyLong_FromLong(self->state);
}

static PyObject *
Shoddy_str_of_obj_concat_str_arg(Shoddy *self, PyObject *args) {
    char *str_arg_val;
    PyObject *obj_str_value;
    PyObject *obj_str_value2;
    PyObject *res;

    if (!PyArg_ParseTuple(args, "s", &str_arg_val))
        return NULL;

    obj_str_value = PyObject_Str(self->obj);
    obj_str_value2 = Py_BuildValue("s", str_arg_val);
    res = PyUnicode_FromFormat("%S %S", obj_str_value, obj_str_value2);
    Py_XDECREF(obj_str_value);
    Py_XDECREF(obj_str_value2);

    return res;
}

static PyObject *
Shoddy_str_of_obj_concat_str_arg2(Shoddy *self, PyObject *args) {
    PyObject *obj_str_value;
    PyObject *obj_str_value2;
    PyObject *res;

    obj_str_value = PyObject_Str(self->obj);
    obj_str_value2 = PyList_GetItem(self->obj, 0);
    res = PyUnicode_FromFormat("%S %S", obj_str_value, obj_str_value2);
    Py_XDECREF(obj_str_value);

    return res;
}


static PyMethodDef Shoddy_methods[] = {
        {"increment",                 (PyCFunction) Shoddy_increment,                 METH_NOARGS,
                PyDoc_STR("increment state counter")},
        {"str_of_obj_concat_str_arg", (PyCFunction) Shoddy_str_of_obj_concat_str_arg, METH_VARARGS,
                PyDoc_STR("str_of_obj_concat_str_arg")},
        {"str_of_obj_concat_str_arg2", (PyCFunction) Shoddy_str_of_obj_concat_str_arg2, METH_VARARGS,
                PyDoc_STR("str_of_obj_concat_str_arg2")},
        {NULL},
};

static PyMemberDef Shoddy_members[] = {
        {"obj", T_OBJECT_EX, offsetof(Shoddy, obj), 0,
                "obj"},
        {NULL}  /* Sentinel */
};

static int
Shoddy_clear(Shoddy *self) {
    PyObject *tmp;

    tmp = self->obj;
    self->obj = NULL;
    Py_XDECREF(tmp);

    return 0;
}

static void
Shoddy_dealloc(Shoddy *self) {
    PyObject_GC_UnTrack(self);
    Shoddy_clear(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
Shoddy_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Shoddy *self;

    self = (Shoddy*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->obj = Py_None;
        self->state = 0;
    }

    return (PyObject *)self;
}

static int
Shoddy_init(Shoddy *self, PyObject *args, PyObject *kwds) {
    if (PyList_Type.tp_init((PyObject *) self, args, kwds) < 0)
        return -1;

    self->state = 0;
    self->obj = Py_None;
    return 0;
}

static int
Shoddy_traverse(Shoddy *self, visitproc visit, void *arg) {
    int vret;

    if (self->obj) {
        vret = visit(self->obj, arg);
        if (vret != 0)
            return vret;
    }

    return 0;
}

static PyTypeObject ShoddyType = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "mymodyle.Mymodyle",         /* tp_name */
        sizeof(Shoddy),          /* tp_basicsize */
        0,                       /* tp_itemsize */
        (destructor) Shoddy_dealloc, /* tp_dealloc */
        0,                       /* tp_print */
        0,                       /* tp_getattr */
        0,                       /* tp_setattr */
        0,                       /* tp_reserved */
        0,                       /* tp_repr */
        0,                       /* tp_as_number */
        0,                       /* tp_as_sequence */
        0,                       /* tp_as_mapping */
        0,                       /* tp_hash */
        0,                       /* tp_call */
        0,                       /* tp_str */
        0,                       /* tp_getattro */
        0,                       /* tp_setattro */
        0,                       /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE |
        Py_TPFLAGS_HAVE_GC,     /* tp_flags */
        0,                       /* tp_doc */
        (traverseproc)Shoddy_traverse, /* tp_traverse */
        (inquiry)Shoddy_clear,  /* tp_clear */
        0,                       /* tp_richcompare */
        0,                       /* tp_weaklistoffset */
        0,                       /* tp_iter */
        0,                       /* tp_iternext */
        Shoddy_methods,          /* tp_methods */
        Shoddy_members,          /* tp_members */
        0,                       /* tp_getset */
        0,                       /* tp_base */
        0,                       /* tp_dict */
        0,                       /* tp_descr_get */
        0,                       /* tp_descr_set */
        0,                       /* tp_dictoffset */
        (initproc)Shoddy_init,   /* tp_init */
        0,                       /* tp_alloc */
        Shoddy_new,             /* tp_new */
};

static PyModuleDef shoddymodule = {
        PyModuleDef_HEAD_INIT,
        "mymodule",
        "mymodule module",
        -1,
        NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_mymodule(void) {
    PyObject *m;

    ShoddyType.tp_base = &PyList_Type;
    if (PyType_Ready(&ShoddyType) < 0)
        return NULL;

    m = PyModule_Create(&shoddymodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&ShoddyType);
    PyModule_AddObject(m, "Mymodule", (PyObject *) &ShoddyType);
    return m;
}
