/*
 * copyright (c)2009 http://wamsoft.jp
 * zlib license
 */
#include "sqthread.h"
#include <string.h>

extern SQRESULT sqstd_loadmemory(HSQUIRRELVM v, const char *dataBuffer, int dataSize, const SQChar *filename, SQBool printerror);

namespace sqobject {

// �p�����[�^���s��
static SQRESULT ERROR_INVALIDPARAM(HSQUIRRELVM v) {
	return sq_throwerror(v, _SC("invalid param"));
}

// �X���b�h�����݂��Ȃ�
static SQRESULT ERROR_NOTHREAD(HSQUIRRELVM v) {
	return sq_throwerror(v, _SC("no thread"));
}

// fork �Ɏ��s����
static SQRESULT ERROR_FORK(HSQUIRRELVM v) {
	return sq_throwerror(v,_SC("failed to fork"));
}

bool
Thread::isWait()
{
	return !_waitSystem.isNull() || _waitList.len() > 0 || _waitTimeout >= 0;
}

/**
 * @return �Y���X���b�h�ƌ��݊Ǘ����̃X���b�h����v���Ă�� true
 */
bool
Thread::isSameThread(HSQUIRRELVM v)
{
	return _thread.isSameThread(v);
}

/**
 * �I�u�W�F�N�g�ɑ΂���҂���������������
 * @param target �҂��Ώ�
 * @return �Y���I�u�W�F�N�g��҂��Ă��ꍇ�� true
 */
bool
Thread::_notifyObject(ObjectInfo &target)
{
	bool find = false;
	if (!_waitSystem.isNull() && _waitSystem == target) { // system�R�}���h��p�̑҂�
		find = true;
		Thread *th = _waitSystem.getThread();
		if (th) {
			_waitResult = th->_exitCode;
		}
		_waitSystem.clear();
	} else {
		SQInteger i = 0;
		SQInteger max = _waitList.len();
		while (i < max) {
			ObjectInfo obj = _waitList.get(i);
			if (obj == target) {
				find = true;
				_waitResult = obj;
				_waitList.remove(i);
				max--;
			} else {
				i++;
			}
		}
	}
	if (find) {
		_clearWait();
	}
	return find;
}

/**
 * �g���K�ɑ΂���҂���������������
 * @param name �g���K��
 * @return �Y���I�u�W�F�N�g��҂��Ă��ꍇ�� true
	 */
bool
Thread::_notifyTrigger(const SQChar *name)
{
	bool find = false;
	int i = 0;
	SQInteger max = _waitList.len();
	while (i < max) {
		ObjectInfo obj = _waitList.get(i);
		if (obj.isSameString(name)) {
			find = true;
			_waitResult = obj;
			_waitList.remove(i);
			max--;
		} else {
			i++;
		}
	}
	if (find) {
		_clearWait();
	}
	return find;
}

// �R���X�g���N�^
Thread::Thread() : _currentTick(0), _fileHandler(NULL), _waitTimeout(-1), _status(THREAD_NONE)
{
	_waitList.initArray();
}

// �R���X�g���N�^
Thread::Thread(HSQUIRRELVM v) : Object(v), _currentTick(0), _fileHandler(NULL), _waitTimeout(-1), _status(THREAD_NONE)
{
	_waitList.initArray();
	// ���s
	if (sq_gettop(v) >= 3) {
		_exec(v,3);
		_entryThread(v);
	}
}

// �f�X�g���N�^
Thread::~Thread()
{
	_exit();
	_thread.clear();
	_waitList.clear();
}

/**
 * ���j��
 */
void
Thread::_init()
{
	HSQUIRRELVM gv = getGlobalVM();
	sq_newthread(gv, 1024);
	_thread.getStack(gv, -1);
	sq_pop(gv, 1);
}

/**
 * ���j��
 */
void
Thread::_clear()
{
	_clearWait();
	if (_fileHandler) {
		sqobjCloseFile(_fileHandler);
		_fileHandler = NULL;
		_scriptName.clear();
	}
	_args.clear();
	_status = THREAD_NONE;
}

void
Thread::_exit()
{
	notifyAll();
	_clear();
}

/**
 * �I�u�W�F�N�g�ɑ΂���҂������N���A����
 * @param status �L�����Z���̏ꍇ�� true
 */
void
Thread::_clearWait()
{
	// system�p�� wait�̉���
	_waitSystem.removeWait(self);
	_waitSystem.clear();

	// ���̑��� wait �̉���
	SQInteger max = _waitList.len();
	for (SQInteger i=0;i<max;i++) {
		_waitList.get(i).removeWait(self);
	}
	_waitList.clearData();
	// �^�C���A�E�g�w��̉���
	_waitTimeout = -1;
}

/**
 * �����p:fork �����B�X���b�h���P�������� VM��PUSH����
 * @param v squirrelVM
 * @return ���������� true
 */
bool
Thread::_fork(HSQUIRRELVM v)
{
	// �X���b�h�I�u�W�F�N�g�̓O���[�o����ɐ�������
	SQInteger max = sq_gettop(v);
	HSQUIRRELVM gv = getGlobalVM();
	sq_pushroottable(gv); // root
	sq_pushstring(gv, SQTHREADNAME, -1);
	if (SQ_SUCCEEDED(sq_get(gv,-2))) { // class
		sq_pushroottable(gv); // ����:self(root)
		sq_pushnull(gv);      // ����:delegate
		// �������R�s�[
		int argc = 2;
		if (gv == v) {
			for (int i=2;i<=max;i++) {
				sq_push(gv,i);
				argc++;
			}
		} else {
			for (int i=2;i<=max;i++) {
				sq_move(gv, v, i);
				argc++;
			}
		}
		if (SQ_SUCCEEDED(sq_call(gv, argc, SQTrue, SQTrue))) { // �R���X�g���N�^�Ăяo��
			if (gv == v) {
				sq_remove(gv, -2); // class
				sq_remove(gv, -2); // root
			} else {
				sq_move(v, gv, sq_gettop(gv)); // ��VM�̂ق��Ɉڂ�
				sq_pop(gv, 3); // thread,class,root
			}
			return true;
		}
		sq_pop(gv, 1); // class
	}
	sq_pop(gv,1); // root
	return false;
}

/**
 * �����p: wait����
 * @param v squirrelVM
 * @param idx �Y�� idx �ȍ~�ɂ�����̂�҂�
 */
void
Thread::_wait(HSQUIRRELVM v, int idx)
{
	_clearWait();
	_waitResult.clear();
	SQInteger max = sq_gettop(v);
	for (int i=idx;i<=max;i++) {
		switch (sq_gettype(v, 2)) {
		case OT_INTEGER:
		case OT_FLOAT:
			// ���l�̏ꍇ�̓^�C���A�E�g�҂�
			{
				SQInteger timeout;
				sq_getinteger(v, i, &timeout);
				if (timeout >= 0) {
					if (_waitTimeout < 0  || _waitTimeout > timeout) {
						_waitResult.getStack(v, i);
						_waitTimeout = timeout;
					}
				}
			}
			break;
		case OT_STRING:
			// �҂����X�g�ɓo�^
			_waitList.append(ObjectInfo(v,i));
			break;
		case OT_INSTANCE:
			// �I�u�W�F�N�g�ɑ҂��o�^���Ă���҂����X�g�ɓo�^
			{
				ObjectInfo o;
				o.getStackWeak(v,i);
				o.addWait(self);
				_waitList.append(o);
			}
			break;
		default:
			break;
		}
	}
}

/**
 * �����p: system�����̑҂��o�^�B�X�^�b�N�擪�ɂ���X���b�h��҂�
 * @param v squirrelVM
 */
void
Thread::_system(HSQUIRRELVM v)
{
	_clearWait();
	_waitResult.clear();
	_waitSystem.getStackWeak(v, -1);
	_waitSystem.addWait(self);
}

// ---------------------------------------------------------------

SQRESULT
Thread::wait(HSQUIRRELVM v)
{
	_wait(v);
	return 0;
}

/**
 * wait�̃L�����Z��
 */
void
Thread::cancelWait()
{
	_clearWait();
	_waitResult.clear();
}

/**
 * �����p: exec����
 * @param v squirrelVM
 * @param idx ���̃C���f�b�N�X�����ɂ�����̂����s�J�n����B������Ȃ�X�N���v�g�A�t�@���N�V�����Ȃ璼��
 */
void
Thread::_exec(HSQUIRRELVM v, int idx)
{
	_clear();
	_thread.clear();
	// �X���b�h�擪�ɃX�N���v�g�����[�h
	if (sq_gettype(v, idx) == OT_STRING) {
		// �X�N���v�g�w��Œx�����[�h
		_scriptName.getStack(v, idx);
		_fileHandler = sqobjOpenFile(getString(v, idx));
		_status = THREAD_LOADING_FILE;
	} else {
		// �t�@���N�V�����w��
		_func.getStack(v, idx);
		_status = THREAD_LOADING_FUNC;
	}

	// �������L�^
	SQInteger max = sq_gettop(v);
	if (max > idx) {
		_args.initArray();
		for (int i=idx+1;i<=max;i++) {
			_args.append(v, i);
		}
	}
}

/**
 * �X���b�h�Ƃ��ēo�^����
 */
void
Thread::_entryThread(HSQUIRRELVM v)
{
	// �X���b�h���Ƃ��ēo�^
	ObjectInfo thinfo(v,1);
	SQInteger max = threadList.len();
	for (int i=0;i<max;i++) {
		if (threadList.get(i) == thinfo) {
			return;
		}
	}
	newThreadList.append(thinfo);
}


/**
 * ���s�J�n
 * @param func ���s�Ώۃt�@���N�V�����B������̏ꍇ�Y���X�N���v�g��ǂݍ���
 */
SQRESULT
Thread::exec(HSQUIRRELVM v)
{
	if (sq_gettop(v) <= 1) {
		return ERROR_INVALIDPARAM(v);
	}
		
	_exec(v);
	_entryThread(v);

	return SQ_OK;
}

/**
 * ���s�I��
 */
SQRESULT
Thread::exit(HSQUIRRELVM v)
{
	if (sq_gettop(v) >= 2) {
		_exitCode.getStack(v,2);
	} else {
		_exitCode.clear();
	}
	_exit();
	return SQ_OK;
}

/**
 * @return ���s�X�e�[�^�X
 */
SQRESULT
Thread::getExitCode(HSQUIRRELVM v)
{
	_exitCode.push(v);
	return 1;
}

/**
 * ���s��~
 */
void
Thread::stop()
{
	if (_status == THREAD_RUN) {
		_status = THREAD_STOP;
	}
}

/**
 * ���s�ĊJ
 */
void
Thread::run()
{
	if (_status == THREAD_STOP) {
		_status = THREAD_RUN;
	}
}

/**
 * @return ���s�X�e�[�^�X
 */
int
Thread::getStatus()
{
	return isWait() ? THREAD_WAIT : _status;
}

/**
 * �X���b�h�̃��C������
 * @param diff �o�ߎ���
 * @return �X���b�h���s�I���Ȃ� true
 */
bool
Thread::_main(long diff)
{
	// �X���b�h�Ƃ��ē���ł��ĂȂ��ꍇ�͑��I��
	if (_status == THREAD_NONE) {
		return true;
	}

	if (_status == THREAD_LOADING_FILE) {
		// �t�@�C���ǂݍ��ݏ���
		const char *dataAddr;
		int dataSize;
		if (sqobjCheckFile(_fileHandler, &dataAddr, &dataSize)) {
			_init();
			SQRESULT ret = sqstd_loadmemory(_thread, dataAddr, dataSize, _scriptName.getString(), SQTrue);
			sqobjCloseFile(_fileHandler);
			_fileHandler = NULL;
			if (SQ_SUCCEEDED(ret)) {
				_status = THREAD_RUN;
			} else {
				// exit����
				printError();
				_exit();
				return true;
			}
		} else {
			// �ǂݍ��݊����҂�
			return false;
		}
	} else if (_status == THREAD_LOADING_FUNC) {
		// �X�N���v�g�ǂݍ��ݏ���
		_init();
		_func.push(_thread);
		_func.clear();
		_status = THREAD_RUN;
	}

	_currentTick += diff;
	
	// �^�C���A�E�g����
	if (_waitTimeout >= 0) {
		_waitTimeout -= diff;
		if (_waitTimeout < 0) {
			_clearWait();
		}
	}
		
	// �X���b�h���s
	if (!isWait() && _status == THREAD_RUN) {
		SQRESULT result;
		if (sq_getvmstate(_thread) == SQ_VMSTATE_SUSPENDED) {
			_waitResult.push(_thread);
			_waitResult.clear();
			result = sq_wakeupvm(_thread, SQTrue, SQFalse, SQTrue, SQFalse);
		} else {
			sq_pushroottable(_thread);
			SQInteger n = _args.pushArray(_thread) + 1;
			_args.clear();
			result = sq_call(_thread, n, SQFalse, SQTrue);
		}
		if (SQ_FAILED(result)) {
			// �X���b�h���G���[�I��
			printError();
			_exit();
		} else  if (sq_getvmstate(_thread) == SQ_VMSTATE_IDLE) {
			// �X���b�h������ɏI��
			if (result > 0) {
				_exitCode.getStack(_thread, -1);
			}
			_exit();
		}
	}
	
	return _status == THREAD_NONE;
}

// -------------------------------------------------------------------------

/**
 * �X���b�h�̃G���[���̕\��
 */
void
Thread::printError()
{
	SQPRINTFUNCTION print = sq_getprintfunc(_thread);
	if (print) {
		sq_getlasterror(_thread);
		const SQChar *err;
		if (SQ_FAILED(sq_getstring(_thread, -1, &err))) {
			err = _SC("unknown");
		}
		print(_thread,_SC("error:%s:%s\n"), _scriptName.getString(), err);
		sq_pop(_thread, 1);
	}
}

/**
 * ����X���b�h�̔j��
 */
void
Thread::init()
{
	threadList.initArray();
	newThreadList.initArray();
}

void
Thread::update(long diff)
{
	diffTick = diff;
	currentTick += diff;
}

/*
 * ���s�������C�����[�v
 * ���ݑ��݂���X���b�h�𑍂Ȃ߂łP�x�������s����B
 * �V�X�e���{�̂̃��C�����[�v(�C�x���g�����{�摜����)
 * ����1�x�����Ăяo�����Ƃŋ@�\����B���ꂼ��̃X���b�h�́A
 * �������疾���I�� suspend() �܂��� wait�n�̃��\�b�h���Ăяo���ď�����
 * ���̃X���b�h�ɈϏ�����K�v������B
 * @return ���쒆�̃X���b�h�̐�
 */
int
Thread::main()
{
	SQInteger i=0;
	SQInteger max = threadList.len();
	while (i < max) {
		Thread *th = threadList.get(i).getThread();
		if (!th || th->_main(diffTick)) {
			threadList.remove(i);
			max--;
		} else {
			i++;
		}
	}
	threadList.appendArray(newThreadList);
	newThreadList.clearData();
	return (int)threadList.len();
};

/**
 * �X�N���v�g���s�J�n�p
 * @param scriptName �X�N���v�g��
 * @param argc �����̐�
 * @param argv ����
 * @return �����Ȃ� true
 */
bool
Thread::fork(const SQChar *scriptName, int argc, const SQChar **argv)
{
	HSQUIRRELVM gv = getGlobalVM();
	sq_pushroottable(gv); // root
	sq_pushstring(gv, SQTHREADNAME, -1);
	if (SQ_SUCCEEDED(sq_get(gv,-2))) { // class
		sq_pushroottable(gv); // ����:self(root)
		sq_pushnull(gv);      // ����:delegate
		sq_pushstring(gv, scriptName, -1); // ����:func
		int n = 3;
		for (int i=0;i<argc;i++) {
			sq_pushstring(gv, argv[i], -1);
			n++;
		}
		if (SQ_SUCCEEDED(sq_call(gv, n, SQTrue, SQTrue))) { // �R���X�g���N�^�Ăяo��
			sq_pop(gv, 3); // thread,class,root
			return true;
		}
		sq_pop(gv, 1); // class
	}
	sq_pop(gv,1); // root
	return false;
}

/**
 * ����X���b�h�̔j��
 */
void
Thread::done()
{
	// �S�X���b�h���������f
	threadList.appendArray(newThreadList);
	newThreadList.clearData();
	SQInteger max = threadList.len();
	for (SQInteger i=0;i<max;i++) {
		Thread *th = threadList.get(i).getThread();
		if (th) { th->_exit();}
	}
	threadList.clearData();
	// ���X�g���̂�j��
	threadList.clear();
	newThreadList.clear();
}

// -------------------------------------------------------------
// �O���[�o���X���b�h�p
// -------------------------------------------------------------

ObjectInfo Thread::threadList; //< �X���b�h�ꗗ
ObjectInfo Thread::newThreadList; //< �V�K�X���b�h�ꗗ
long Thread::currentTick = 0;  //< ���݂̃V�X�e��tick
long Thread::diffTick = 0;  //< ����̌Ăяo������

// -------------------------------------------------------------
// �O���[�o�����\�b�h�p
// -------------------------------------------------------------

/**
 * ���ݎ����̎擾
 */
SQRESULT
Thread::global_getCurrentTick(HSQUIRRELVM v)
{
	sq_pushinteger(v, currentTick);
	return 1;
}

/**
 * ���������̎擾
 */
SQRESULT
Thread::global_getDiffTick(HSQUIRRELVM v)
{
	sq_pushinteger(v, diffTick);
	return 1;
}

/*
 * @return ���݂̃X���b�h��Ԃ�
 */
SQRESULT
Thread::global_getCurrentThread(HSQUIRRELVM v)
{
	SQInteger max = threadList.len();
	for (SQInteger i=0;i<max;i++) {
		ObjectInfo obj = threadList.get(i);
		Thread *th = obj.getThread();
		if (th && th->isSameThread(v)) {
			obj.push(v);
			return 1;
		}
	}
	return ERROR_NOTHREAD(v);
}

/*
 * @return ���݂̃X���b�h�ꗗ��Ԃ�
 */
SQRESULT
Thread::global_getThreadList(HSQUIRRELVM v)
{
	threadList.pushClone(v);
	return 1;
}

/*
 * �X�N���v�g��V�����X���b�h�Ƃ��Ď��s����
 * �� return Thread(func); ����
 * @param func �X���b�h�Ŏ��s����t�@���N�V����
 * @return �V�X���b�h
 */
SQRESULT
Thread::global_fork(HSQUIRRELVM v)
{
	if (!_fork(v)) {
		return ERROR_FORK(v);
	}
	return 1;
}

/**
 * @return ���ݎ��s���̃X���b�h���I�u�W�F�N�g(Thread*)
 */
Thread *
Thread::getCurrentThread(HSQUIRRELVM v)
{
	SQInteger max = threadList.len();
	for (SQInteger i=0;i<max;i++) {
		ObjectInfo obj = threadList.get(i);
		Thread *th = obj.getThread();
		if (th && th->isSameThread(v)) {
			return th;
		}
	}
	return NULL;
}

/**
 * �X�N���v�g��؂�ւ���
 * @param func �X���b�h�Ŏ��s����t�@���N�V����
 */
SQRESULT
Thread::global_exec(HSQUIRRELVM v)
{
	Thread *th = getCurrentThread(v);
	if (!th) {
		return ERROR_NOTHREAD(v);
	}
	if (sq_gettop(v) <= 1) {
		return ERROR_INVALIDPARAM(v);
	}
	th->_exec(v);
	return sq_suspendvm(v);
}

/**
 * ���s���X���b�h�̏I��
 */
SQRESULT
Thread::global_exit(HSQUIRRELVM v)
{
	Thread *th = getCurrentThread(v);
	if (!th) {
		return ERROR_NOTHREAD(v);
	}
	th->exit(v);
	return sq_suspendvm(v);
}

/**
 * �R�}���h���s
 * @param func �X���b�h�Ŏ��s����t�@���N�V����
 * @return �I���R�[�h
 */
SQRESULT
Thread::global_system(HSQUIRRELVM v)
{
	Thread *th = getCurrentThread(v);
	if (!th) {
		return ERROR_NOTHREAD(v);
	}
	if (!_fork(v)) {
		return ERROR_FORK(v);
	}
	th->_system(v);
	sq_pop(v,1);
	return sq_suspendvm(v);
}

/**
 * ���s���X���b�h�̏����҂�
 * @param target int:���ԑ҂�(ms), string:�g���K�҂�, obj:�I�u�W�F�N�g�҂�
 * @param timeout �^�C���A�E�g(�ȗ����͖����ɑ҂�)
 * @return �҂����L�����Z�����ꂽ�� true
 */
SQRESULT
Thread::global_wait(HSQUIRRELVM v)
{
	Thread *th = getCurrentThread(v);
	if (!th) {
		return ERROR_NOTHREAD(v);
	}
	th->_wait(v);
	return sq_suspendvm(v);
}

/**
 * �S�X���b�h�ւ̃g���K�ʒm
 * @param name �����҂��g���K��
 */
SQRESULT
Thread::global_trigger(HSQUIRRELVM v)
{
	const SQChar *name = getString(v, 2);
	SQInteger max = threadList.len();
	for (SQInteger i=0;i<max;i++) {
		ObjectInfo obj = threadList.get(i);
		obj.notifyTrigger(name);
	}
	return SQ_OK;
}

/**
 * �O���[�o�����\�b�h�o�^
 */
void
Thread::registerGlobal()
{
	// ���\�b�h�o�^�i���O���j
#define REGISTERMETHODNAME(name, method) \
	sq_pushstring(v, _SC(#name), -1);\
	sq_newclosure(v, method, 0);\
	sq_createslot(v, -3);

	// enum �o�^�i���O���j
#define REGISTERENUM(name, value) \
	sq_pushstring(v, _SC(#name), -1); /* ���O�� push */ \
	sq_pushinteger(v, value);          /* �l�� push */ \
	sq_createslot(v, -3)              /* �e�[�u���ɓo�^ */
	
	HSQUIRRELVM v = getGlobalVM();

	// �O���[�o�����\�b�h�̓o�^
	sq_pushroottable(v); // root
	REGISTERMETHODNAME(getCurrentTick, global_getCurrentTick);
	REGISTERMETHODNAME(getDiffTick, global_getDiffTick);
	REGISTERMETHODNAME(getCurrentThread, global_getCurrentThread);
	REGISTERMETHODNAME(getThreadList, global_getThreadList);
	REGISTERMETHODNAME(fork, global_fork);
	REGISTERMETHODNAME(exec, global_exec);
	REGISTERMETHODNAME(exit, global_exit);
	REGISTERMETHODNAME(system, global_system);
	REGISTERMETHODNAME(wait, global_wait);
	REGISTERMETHODNAME(notify, global_trigger);
	sq_pop(v, 1); // root
	
	// �萔�̓o�^
	sq_pushconsttable(v); // consttable
	sq_pushstring(v, _SC("THREADSTATUS"), -1); // �e�[�u������ push
	sq_newtable(v);                  // �V���� enum �e�[�u��
	REGISTERENUM(NONE,THREAD_NONE);
	REGISTERENUM(LOADING_FILE,THREAD_LOADING_FILE);
	REGISTERENUM(LOADING_FUNC,THREAD_LOADING_FUNC);
	REGISTERENUM(STOP,THREAD_STOP);
	REGISTERENUM(RUN,THREAD_RUN);
	REGISTERENUM(WAIT,THREAD_WAIT);
	sq_createslot(v, -3);              /* �e�[�u���ɓo�^ */
	sq_pop(v, 1); // consttable
}

};