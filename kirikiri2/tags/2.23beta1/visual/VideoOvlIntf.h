//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2004  W.Dee <dee@kikyou.info>

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Video Overlay support interface
//---------------------------------------------------------------------------
#ifndef VideoOvlIntfH
#define VideoOvlIntfH

#include "tjsNative.h"
#include "SoundBufferBaseIntf.h"

//---------------------------------------------------------------------------
// tTJSNI_BaseVideoOverlay
//---------------------------------------------------------------------------
class tTJSNI_Window;
class tTJSNI_BaseVideoOverlay : public tTJSNativeInstance
{
	typedef tTJSNativeInstance inherited;

public:
	tTJSNI_BaseVideoOverlay();
	tjs_error TJS_INTF_METHOD
	Construct(tjs_int numparams, tTJSVariant **param,
		iTJSDispatch2 *tjs_obj);
	void TJS_INTF_METHOD Invalidate();

protected:
	iTJSDispatch2 *Owner;
	bool CanDeliverEvents;
	tTJSNI_Window * Window;
	tTJSVariantClosure ActionOwner;
	tTVPSoundStatus Status; // status

	ttstr GetStatusString() const;
	void SetStatus(tTVPSoundStatus s);
	void SetStatusAsync(tTVPSoundStatus s);
	void FireCallbackCommand(const ttstr & command, const ttstr & argument);


public:
	virtual void Disconnect() = 0; // called from Window object's invalidation

	tTJSVariantClosure GetActionOwnerNoAddRef() const { return ActionOwner; }
};
//---------------------------------------------------------------------------

#include "VideoOvlImpl.h" // must define tTJSNI_VideoOverlay class

//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// tTJSNC_VideoOverlay : TJS VideoOverlay class
//---------------------------------------------------------------------------
class tTJSNC_VideoOverlay : public tTJSNativeClass
{
public:
	tTJSNC_VideoOverlay();
	static tjs_uint32 ClassID;

protected:
	tTJSNativeInstance *CreateNativeInstance();
};
//---------------------------------------------------------------------------
extern tTJSNativeClass * TVPCreateNativeClass_VideoOverlay();
//---------------------------------------------------------------------------
#endif
