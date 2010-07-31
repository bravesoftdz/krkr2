/*
 * copyright (c)2009 http://wamsoft.jp
 * zlib license
 */
#include "sqobjectinfo.h"
#include "sqobject.h"
#include <string.h>
#include <sqstdstring.h>

namespace sqobject {

bool
pushObject(HSQUIRRELVM v, Object *obj)
{
	if (obj->isInit()) {
		obj->push(v);
		return true;
	}
	return false;
}

// roottable �̎擾
ObjectInfo
ObjectInfo::getRoot()
{
	HSQUIRRELVM gv = getGlobalVM();
	sq_pushroottable(gv); // root
	ObjectInfo ret(gv, -1);
	sq_pop(gv, 1);
	return ret;
}

ObjectInfo
ObjectInfo::createArray(SQInteger size)
{
	ObjectInfo ret;
	ret.initArray(size);
	return ret;
}

ObjectInfo
ObjectInfo::createTable()
{
	ObjectInfo ret;
	ret.initTable();
	return ret;
}

// ��r
bool
ObjectInfo::operator == (const ObjectInfo &o)
{
	bool cmp = false;
	HSQUIRRELVM v = getGlobalVM();
	push(v);
	o.push(v);
	cmp = sq_cmp(v) == 0;
	sq_pop(v,2);
	return cmp;
}

bool
ObjectInfo::operator !=(const ObjectInfo& o)
{
	bool cmp = false;
	HSQUIRRELVM v = getGlobalVM();
	push(v);
	o.push(v);
	cmp = sq_cmp(v) != 0;
	sq_pop(v,2);
	return cmp;
}

bool
ObjectInfo::operator <(const ObjectInfo& o)
{
	bool cmp = false;
	HSQUIRRELVM v = getGlobalVM();
	push(v);
	o.push(v);
	cmp = sq_cmp(v) < 0;
	sq_pop(v,2);
	return cmp;
}

bool
ObjectInfo::operator <=(const ObjectInfo& o)
{
	bool cmp = false;
	HSQUIRRELVM v = getGlobalVM();
	push(v);
	o.push(v);
	cmp = sq_cmp(v) <= 0;
	sq_pop(v,2);
	return cmp;
}

bool
ObjectInfo::operator >(const ObjectInfo& o)
{
	bool cmp = false;
	HSQUIRRELVM v = getGlobalVM();
	push(v);
	o.push(v);
	cmp = sq_cmp(v) > 0;
	sq_pop(v,2);
	return cmp;
}

bool
ObjectInfo::operator >=(const ObjectInfo& o)
{
	bool cmp = false;
	HSQUIRRELVM v = getGlobalVM();
	push(v);
	o.push(v);
	cmp = sq_cmp(v) >= 0;
	sq_pop(v,2);
	return cmp;
}

// ���e����
void
ObjectInfo::clear()
{
	HSQUIRRELVM gv = getGlobalVM();
	sq_release(gv,&obj);
	sq_resetobject(&obj);
}

// �X�^�b�N����擾
void
ObjectInfo::getStack(HSQUIRRELVM v, SQInteger idx)
{
	clear();
	HSQUIRRELVM gv = getGlobalVM();
	sq_move(gv, v, idx);
	sq_getstackobj(gv, -1, &obj);
	sq_addref(gv, &obj);
	sq_pop(gv, 1);
}

// �X�^�b�N�����Q�ƂƂ��Ď擾
void
ObjectInfo::getStackWeak(HSQUIRRELVM v, SQInteger idx)
{
	clear();
	HSQUIRRELVM gv = getGlobalVM();
	sq_weakref(v, idx);
	sq_move(gv, v, -1);
	sq_pop(v, 1);
	sq_getstackobj(gv, -1, &obj);
	sq_addref(gv, &obj);
	sq_pop(gv, 1);
}


// �R���X�g���N�^
ObjectInfo::ObjectInfo() {
	sq_resetobject(&obj);
}

// �R���X�g���N�^
ObjectInfo::ObjectInfo(HSQOBJECT obj) : obj(obj)
{
	HSQUIRRELVM gv = getGlobalVM();
	sq_addref(gv, &obj);
}

// �R���X�g���N�^
ObjectInfo::ObjectInfo(HSQUIRRELVM v, SQInteger idx)
{
	sq_resetobject(&obj);
	HSQUIRRELVM gv = getGlobalVM();
	sq_move(gv, v, idx);
	sq_getstackobj(gv, -1, &obj);
	sq_addref(gv, &obj);
	sq_pop(gv, 1);
}

// �R�s�[�R���X�g���N�^
ObjectInfo::ObjectInfo(const ObjectInfo &orig)
{
	HSQUIRRELVM gv = getGlobalVM();
	sq_resetobject(&obj);
	obj = orig.obj;
	sq_addref(gv, &obj);
}

// ���
ObjectInfo& ObjectInfo::operator =(const ObjectInfo &orig)
{
	HSQUIRRELVM gv = getGlobalVM();
	clear();
	obj = orig.obj;
	sq_addref(gv, &obj);
	return *this;
}

// �f�X�g���N�^
ObjectInfo::~ObjectInfo()
{
	clear();
}

// null���H
bool
ObjectInfo::isNull() const
{
	if (sq_isweakref(obj)) {
		HSQUIRRELVM gv = getGlobalVM();
		sq_pushobject(gv, obj);
		sq_getweakrefval(gv, -1);
		bool ret = sq_gettype(gv, -1) == OT_NULL;
		sq_pop(gv, 2);
		return ret;
	}
	return sq_isnull(obj);
}

// �����X���b�h���H
bool
ObjectInfo::isSameThread(const HSQUIRRELVM v) const
{
	return sq_isthread(obj) && obj._unVal.pThread == v;
}

// �X���b�h���擾
ObjectInfo::operator HSQUIRRELVM() const
{
	HSQUIRRELVM vm = sq_isthread(obj) ? obj._unVal.pThread : NULL;
	return vm;
}
	
// �I�u�W�F�N�g��PUSH
void
ObjectInfo::push(HSQUIRRELVM v) const
{
    if (sq_isweakref(obj)) {
		sq_pushobject(v, obj);
		sq_getweakrefval(v, -1);
		sq_remove(v, -2);
	} else {
		sq_pushobject(v, obj);
	}
}

/// ������o�^(���s������null��o�^
void
ObjectInfo::pushClone(HSQUIRRELVM v) const
{
	push(v);
	if (!SQ_SUCCEEDED(sq_clone(v, -1))) {
		sq_pushnull(v);
	}
	sq_remove(v, -2);
}

// ---------------------------------------------------
// delegate �����p
// ---------------------------------------------------

// delegate �Ƃ��ċ@�\���邩�ǂ���
bool
ObjectInfo::isDelegate() const
{
	return (sq_isinstance(obj) || sq_istable(obj));
}

// bindenv �����邩�ǂ���
bool
ObjectInfo::isBindDelegate() const
{
	return (sq_isinstance(obj));
}

// ---------------------------------------------------
// �f�[�^�擾
// ---------------------------------------------------

const SQChar *
ObjectInfo::getString()
{
	if (sq_isstring(obj)) {
		HSQUIRRELVM gv = getGlobalVM();
		push(gv);
		const SQChar *mystr;
		sq_getstring(gv, -1, &mystr);
		sq_pop(gv, 1);
		return mystr;
	}
	return NULL;
}

// ---------------------------------------------------
// wait�����p���\�b�h
// ---------------------------------------------------

bool
ObjectInfo::isSameString(const SQChar *str) const
{
	if (str && sq_isstring(obj)) {
		HSQUIRRELVM gv = getGlobalVM();
		push(gv);
		const SQChar *mystr;
		sq_getstring(gv, -1, &mystr);
		sq_pop(gv, 1);
		return mystr && scstrcmp(str, mystr) == 0;
	}
	return false;
}

// ---------------------------------------------------
// �z�񏈗��p���\�b�h
// ---------------------------------------------------


/// �z��Ƃ��ď�����
void
ObjectInfo::initArray(SQInteger size)
{
	clear();
	HSQUIRRELVM gv = getGlobalVM();
	sq_newarray(gv, size);
	sq_getstackobj(gv, -1, &obj);
	sq_addref(gv, &obj);
	sq_pop(gv, 1);
}

/// �z��Ƃ��ď�����
void
ObjectInfo::initTable()
{
	clear();
	HSQUIRRELVM gv = getGlobalVM();
	sq_newtable(gv);
	sq_getstackobj(gv, -1, &obj);
	sq_addref(gv, &obj);
	sq_pop(gv, 1);
}

/// �z��ɒl��ǉ�
SQRESULT ObjectInfo::append(HSQUIRRELVM v, SQInteger idx)
{
	HSQUIRRELVM gv = getGlobalVM();
	push(gv);
	sq_move(gv, v, idx);
	SQRESULT ret = sq_arrayappend(gv, -2);
	sq_pop(gv,1);
	return ret;
}

/// �z��ɔz���ǉ�
SQRESULT ObjectInfo::appendArray(ObjectInfo &array)
{
	SQRESULT result = SQ_OK;
	HSQUIRRELVM gv = getGlobalVM();
	push(gv);
	SQInteger max = array.len();
	for (SQInteger i=0;i<max;i++) {
		array.pushData(gv, i);
		if (SQ_FAILED(result = sq_arrayappend(gv, -2))) {
			break;
		}
	}
	sq_pop(gv,1);
	return result;
}

/// �z��̒���
SQInteger
ObjectInfo::len() const
{
	HSQUIRRELVM gv = getGlobalVM();
	push(gv);
	SQInteger ret = sq_getsize(gv,-1);
	sq_pop(gv,1);
	return ret;
}

/**
 * �z��̓��e��S��PUSH
 * @param v squirrelVM
 * @return push ������
 */
SQInteger
ObjectInfo::pushArray(HSQUIRRELVM v) const
{
	if (!isArray()) {
		return 0;
	}
	HSQUIRRELVM gv = getGlobalVM();
	push(gv);
	SQInteger len = sq_getsize(gv,-1);
	for (SQInteger i=0;i<len;i++) {
		sq_pushinteger(gv, i);
		if (SQ_SUCCEEDED(sq_get(gv, -2))) {
			sq_move(v, gv, -1);
			sq_pop(gv, 1);
		}
	}
	sq_pop(gv,1);
	return len;
}

// ---------------------------------------------------
// �֐������p���\�b�h
// ---------------------------------------------------

SQRESULT ObjectInfo::call(ObjectInfo *self)
{
	HSQUIRRELVM gv = getGlobalVM();
	push(gv);
	if (self) {
		self->push(gv);
	} else {
		sq_pushroottable(gv); // root
	}
	SQRESULT ret = sq_call(gv, 1, SQFalse, SQTrue);
	sq_pop(gv, 1);
	return ret;
}

// -------------------------------------------------------------
// �N���X�����p
// -------------------------------------------------------------

bool
ObjectInfo::isClass() const
{
	return sq_isclass(obj);
}

// ---------------------------------------------------
// �����񏈗���
// ---------------------------------------------------

std::basic_string<SQChar>
ObjectInfo::toString() const
{
	std::basic_string<SQChar> ret;
	HSQUIRRELVM gv = getGlobalVM();
	push(gv);
	sq_tostring(gv, -1);
	const SQChar *str;
	if (SQ_SUCCEEDED(sq_getstring(gv, -1, &str))) {
		ret = str;
	}
	sq_pop(gv, 2);
	return ret;
}

int
ObjectInfo::printf(const SQChar *format)
{
	HSQUIRRELVM gv = getGlobalVM();
	sq_pushstring(gv, format, -1);
	return _printf(gv, 0);
}

int
ObjectInfo::_printf(HSQUIRRELVM v, int argnum)
{
	int n = sq_gettop(v);
	SQChar *dest = NULL;
	SQInteger length = 0;
	if(SQ_SUCCEEDED(sqstd_format(v,n-argnum,&length,&dest))) {
		SQPRINTFUNCTION func = sq_getprintfunc(v);
		if (func) {
			func(v, _SC("%*s"), length, dest);
		}
	}
	sq_pop(v, argnum+1);
	return length;
}

// �l�� push
void pushValue(HSQUIRRELVM v, bool value) { sq_pushbool(v,value ? SQTrue : SQFalse); }
void pushValue(HSQUIRRELVM v, SQBool value) { sq_pushbool(v,value); }
void pushValue(HSQUIRRELVM v, SQInteger value) { sq_pushinteger(v,value); }
void pushValue(HSQUIRRELVM v, SQFloat value) { sq_pushfloat(v,value); }
void pushValue(HSQUIRRELVM v, const SQChar *value) { if (value) {sq_pushstring(v,value,-1);} else { sq_pushnull(v);} }
void pushValue(HSQUIRRELVM v, SQUserPointer value) { if (value) {sq_pushuserpointer(v,value);} else { sq_pushnull(v);} }
void pushValue(HSQUIRRELVM v, ObjectInfo &obj) { obj.push(v); }
void pushValue(HSQUIRRELVM v, std::basic_string<SQChar> &value) { sq_pushstring(v,value.c_str(),value.length()); }
void pushValue(HSQUIRRELVM v, SQFUNCTION func) { sq_newclosure(v, func, 0); }

// �l�̎擾
SQRESULT getValue(HSQUIRRELVM v, bool *value) { SQBool b;SQRESULT ret = sq_getbool(v, -1, &b); *value = b != SQFalse; return ret; }
SQRESULT getValue(HSQUIRRELVM v, SQBool *value) { return sq_getbool(v, -1, value); }
SQRESULT getValue(HSQUIRRELVM v, SQInteger *value) { return sq_getinteger(v, -1, value); }
SQRESULT getValue(HSQUIRRELVM v, SQFloat *value) { return sq_getfloat(v, -1, value); }
SQRESULT getValue(HSQUIRRELVM v, const SQChar **value) { return sq_getstring(v, -1, value); }
SQRESULT getValue(HSQUIRRELVM v, SQUserPointer *value) { return sq_getuserpointer(v, -1, value); }
SQRESULT getValue(HSQUIRRELVM v, ObjectInfo *value) { value->getStack(v,-1); return SQ_OK; }
SQRESULT getValue(HSQUIRRELVM v, std::basic_string<SQChar> *value) {const SQChar *str; SQRESULT ret; if (SQ_SUCCEEDED((ret = sq_getstring(v, -1, &str)))) { *value = str;} return ret;}

// �l�̋���������
void clearValue(bool *value) { *value = false; }
void clearValue(SQBool *value) { *value = SQFalse; }
void clearValue(SQInteger *value) { *value = 0; }
void clearValue(SQFloat *value) { *value = 0.0f; }
void clearValue(const SQChar **value) { *value = 0; }
void clearValue(SQUserPointer *value) { *value = 0; }
void clearValue(ObjectInfo *value) { value->clear(); }
void clearValue(std::basic_string<SQChar> *value) { *value = _SC(""); }

// �l�̎擾�F��{���̃R�s�y�B������͈��S�łȂ��ꍇ������̂Ŕr������K�v����
SQRESULT getResultValue(HSQUIRRELVM v, bool *value) { SQBool b;SQRESULT ret = sq_getbool(v, -1, &b); *value = b != SQFalse; return ret; }
SQRESULT getResultValue(HSQUIRRELVM v, SQBool *value) { return sq_getbool(v, -1, value); }
SQRESULT getResultValue(HSQUIRRELVM v, SQInteger *value) { return sq_getinteger(v, -1, value); }
SQRESULT getResultValue(HSQUIRRELVM v, SQFloat *value) { return sq_getfloat(v, -1, value); }
SQRESULT getResultValue(HSQUIRRELVM v, SQUserPointer *value) { return sq_getuserpointer(v, -1, value); }
SQRESULT getResultValue(HSQUIRRELVM v, ObjectInfo *value) { value->getStack(v,-1); return SQ_OK; }
SQRESULT getResultValue(HSQUIRRELVM v, std::basic_string<SQChar> *value) {const SQChar *str; SQRESULT ret; if (SQ_SUCCEEDED((ret = sq_getstring(v, -1, &str)))) { *value = str;} return ret;}

};