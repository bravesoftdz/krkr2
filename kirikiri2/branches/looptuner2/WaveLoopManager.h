#ifndef WaveLoopManagerH
#define WaveLoopManagerH
//---------------------------------------------------------------------------

#include "tjsTypes.h"
#include "WaveReader.h"
#include <vector>
#include <string>

#define TVP_WL_MAX_FLAGS 16

#define TVP_WL_SMOOTH_TIME 50
#define TVP_WL_SMOOTH_TIME_HALF (TVP_WL_SMOOTH_TIME/2)

#define TVP_WL_MAX_ID_LEN 16


//---------------------------------------------------------------------------
#ifdef TVP_IN_LOOP_TUNER
	typedef AnsiString tTVPLabelStringType;
#else
	typedef ttstr tTVPLabelStringType;
#endif
//---------------------------------------------------------------------------


#ifdef TVP_IN_LOOP_TUNER
	//---------------------------------------------------------------------------
	// tTJSCriticalSection ( taken from tjsUtils.h )
	//---------------------------------------------------------------------------
	class tTJSCriticalSection
	{
		CRITICAL_SECTION CS;
	public:
		tTJSCriticalSection() { InitializeCriticalSection(&CS); }
		~tTJSCriticalSection() { DeleteCriticalSection(&CS); }

		void Enter() { EnterCriticalSection(&CS); }
		void Leave() { LeaveCriticalSection(&CS); }
	};
	//---------------------------------------------------------------------------
	// tTJSCriticalSectionHolder ( taken from tjsUtils.h )
	//---------------------------------------------------------------------------
	class tTJSCriticalSectionHolder
	{
		tTJSCriticalSection *Section;
	public:
		tTJSCriticalSectionHolder(tTJSCriticalSection &cs)
		{
			Section = &cs;
			Section->Enter();
		}

		~tTJSCriticalSectionHolder()
		{
			Section->Leave();
		}
	};
#else
	#include "tjsUtils.h"
#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// tTVPWaveLoopLink : link structure
//---------------------------------------------------------------------------
enum tTVPWaveLoopLinkCondition
{
	llcNone,
	llcEqual,
	llcNotEqual,
	llcGreater,
	llcGreaterOrEqual,
	llcLesser,
	llcLesserOrEqual
};
//---------------------------------------------------------------------------
struct tTVPWaveLoopLink
{
	tjs_int64 From;		// 'From' in sample position
	tjs_int64 To;		// 'To' in sample position
	bool Smooth;		// Smooth transition (uses short 50ms crossfade)
	tTVPWaveLoopLinkCondition Condition;	// Condition
	tjs_int RefValue;
	tjs_int CondVar;	// Condition variable index
#ifdef TVP_IN_LOOP_TUNER
	// these are only used by the loop tuner
	tjs_int FromTier;	// display tier of vertical 'from' line
	tjs_int LinkTier;	// display tier of horizontal link
	tjs_int ToTier;		// display tier of vertical 'to' allow line
	tjs_int Index;		// link index

	struct tSortByDistanceFuncObj
	{
		bool operator()(
			const tTVPWaveLoopLink &lhs,
			const tTVPWaveLoopLink &rhs) const
		{
			tjs_int64 lhs_dist = lhs.From - lhs.To;
			if(lhs_dist < 0) lhs_dist = -lhs_dist;
			tjs_int64 rhs_dist = rhs.From - rhs.To;
			if(rhs_dist < 0) rhs_dist = -rhs_dist;
			return lhs_dist < rhs_dist;
		}
	};

	struct tSortByIndexFuncObj
	{
		bool operator()(
			const tTVPWaveLoopLink &lhs,
			const tTVPWaveLoopLink &rhs) const
		{
			return lhs.Index < rhs.Index;
		}
	};
#endif
};
//---------------------------------------------------------------------------
bool inline operator < (const tTVPWaveLoopLink & lhs, const tTVPWaveLoopLink & rhs)
{
	if(lhs.From < rhs.From) return true;
	if(lhs.From == rhs.From)
	{
		// give priority to conditional expression
		return lhs.CondVar > rhs.CondVar;
	}
	return false;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// tTVPWaveLoopLink : label structure
//---------------------------------------------------------------------------
struct tTVPWaveLabel
{
	tjs_int64 Position; // label position
	tTVPLabelStringType Name; // label name
#ifdef TVP_IN_LOOP_TUNER
	// these are only used by the loop tuner
	tjs_int NameWidth; // display name width
	tjs_int Index; // index

	struct tSortByPositionFuncObj
	{
		bool operator()(
			const tTVPWaveLabel &lhs,
			const tTVPWaveLabel &rhs) const
		{
			return lhs.Position < rhs.Position;
		}
	};

	struct tSortByIndexFuncObj
	{
		bool operator()(
			const tTVPWaveLabel &lhs,
			const tTVPWaveLabel &rhs) const
		{
			return lhs.Index < rhs.Index;
		}
	};

#endif
};
//---------------------------------------------------------------------------
bool inline operator < (const tTVPWaveLabel & lhs, const tTVPWaveLabel & rhs)
{
	return lhs.Position < rhs.Position;
}

//---------------------------------------------------------------------------
struct tTVPWaveLoopSegment
{
	tTVPWaveLoopSegment(tjs_int64 start, tjs_int64 length)
		{ Start = start; Length = length; }
	tjs_int64 Start;
	tjs_int64 Length;
};
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// tTVPWaveLoopManager : wave loop manager
//---------------------------------------------------------------------------
class tTVPWaveLoopManager
{
	tTJSCriticalSection FlagsCS; // CS to protect flags/links/labels
	int Flags[TVP_WL_MAX_FLAGS];
	std::vector<tTVPWaveLoopLink> Links;
	std::vector<tTVPWaveLabel> Labels;
	tTJSCriticalSection DataCS; // CS to protect other members
	tTVPWaveFormat Format;
	tTVPWaveDecoder * Decoder;

	tjs_int ShortCrossFadeHalfSamples;
		// TVP_WL_SMOOTH_TIME_HALF in sample unit

	tjs_int64 Position; // decoding position

	tjs_uint8 *CrossFadeSamples; // sample buffer for crossfading
	tjs_int CrossFadeLen;
	tjs_int CrossFadePosition;

	bool IsLinksSorted; // false if links are not yet sorted
	bool IsLabelsSorted; // false if labels are not yet sorted

public:
	tTVPWaveLoopManager(tTVPWaveDecoder * decoder);
	virtual ~tTVPWaveLoopManager();

	bool GetFlag(tjs_int index);
	void SetFlag(tjs_int index, int f);
	void ClearFlags();
	void ClearLinksAndLabels();

	const std::vector<tTVPWaveLoopLink> & GetLinks() const;
	const std::vector<tTVPWaveLabel> & GetLabels() const;

	void SetLinks(const std::vector<tTVPWaveLoopLink> & links);
	void SetLabels(const std::vector<tTVPWaveLabel> & labels);

	tjs_int64 GetPosition() const;
	void SetPosition(tjs_int64 pos);

	void Decode(void *dest, tjs_uint samples, tjs_uint &written,
		std::vector<tTVPWaveLoopSegment> &segments,
		std::vector<tTVPWaveLabel> &labels);


private:
	bool GetNearestEvent(tjs_int64 current,
		tTVPWaveLoopLink & link, bool ignore_conditions);

	void GetLabelAt(tjs_int64 from, tjs_int64 to,
		std::vector<tTVPWaveLabel> & labels);

	void DoCrossFade(void *dest, void *src1, void *src2, tjs_int samples,
		tjs_int ratiostart, tjs_int ratioend);

	void ClearCrossFadeInformation();
//--- loop information input/output stuff
private:
	static bool GetInt(char *s, tjs_int &v);
	static bool GetInt64(char *s, tjs_int64 &v);
	static bool GetBool(char *s, bool &v);
	static bool GetCondition(char *s, tTVPWaveLoopLinkCondition &v);
	static bool GetString(char *s, tTVPLabelStringType &v);

	static bool GetEntityToken(char * & p, char **name, char **value);

	static bool ReadLinkInformation(char * & p, tTVPWaveLoopLink &link);
	static bool ReadLabelInformation(char * & p, tTVPWaveLabel &label);
public:
	bool ReadInformation(char * p);

#ifdef TVP_IN_LOOP_TUNER
	// output facility (currently only available with VCL interface)
private:
	static void PutInt(AnsiString &s, tjs_int v);
	static void PutInt64(AnsiString &s, tjs_int64 v);
	static void PutBool(AnsiString &s, bool v);
	static void PutCondition(AnsiString &s, tTVPWaveLoopLinkCondition v);
	static void PutString(AnsiString &s, tTVPLabelStringType v);
	static void DoSpacing(AnsiString &l, int col);
public:
	void WriteInformation(AnsiString &s);
#endif


public:
};
//---------------------------------------------------------------------------
#endif


