// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_syscalls.c -- this file is only included when building a dll
// cg_syscalls.asm is included instead when building a qvm
#include "cg_local.h"
#include "cg_multispec.h"

static intptr_t (QDECL *Q_syscall)( intptr_t arg, ... ) = (intptr_t (QDECL *)( intptr_t, ...))-1;

Q_EXPORT void dllEntry( intptr_t (QDECL  *syscallptr)( intptr_t arg,... ) ) {
	Q_syscall = syscallptr;
}


int PASSFLOAT( float x ) {
	floatint_t fi;
	fi.f = x;
	return fi.i;
}

void	trap_Print( const char *fmt ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_PRINT, fmt );
}

void	trap_Error( const char *fmt ) {
	Q_syscall( CG_ERROR, fmt );
	// shut up GCC warning about returning functions, because we know better
	exit(1);
}

int		trap_Milliseconds( void ) {
	return Q_syscall( CG_MILLISECONDS ); 
}

//rww - precision timer funcs... -ALWAYS- call end after start with supplied ptr, or you'll get a nasty memory leak.
//not that you should be using these outside of debug anyway.. because you shouldn't be. So don't.

//Start should be suppled with a pointer to an empty pointer (e.g. void *blah; trap_PrecisionTimer_Start(&blah);),
//the empty pointer will be filled with an exe address to our timer (this address means nothing in vm land however).
//You must pass this pointer back unmodified to the timer end func.
void trap_PrecisionTimer_Start(void **theNewTimer)
{
	Q_syscall(CG_PRECISIONTIMER_START, theNewTimer);
}

//If you're using the above example, the appropriate call for this is int result = trap_PrecisionTimer_End(blah);
int trap_PrecisionTimer_End(void *theTimer)
{
	return Q_syscall(CG_PRECISIONTIMER_END, theTimer);
}

void	trap_Cvar_Register( vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags ) {
	Q_syscall( CG_CVAR_REGISTER, vmCvar, varName, defaultValue, flags );
}

void	trap_Cvar_Update( vmCvar_t *vmCvar ) {
	Q_syscall( CG_CVAR_UPDATE, vmCvar );
}

void	trap_Cvar_Set( const char *var_name, const char *value ) {
	Q_syscall( CG_CVAR_SET, var_name, value );
}

void trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize ) {
	Q_syscall( CG_CVAR_VARIABLESTRINGBUFFER, var_name, buffer, bufsize );
}

int trap_Cvar_GetHiddenVarValue(const char *name)
{
	return Q_syscall(CG_CVAR_GETHIDDENVALUE, name);
}

int		trap_Argc( void ) {
	return Q_syscall( CG_ARGC );
}

void	trap_Argv( int n, char *buffer, int bufferLength ) {
	Q_syscall( CG_ARGV, n, buffer, bufferLength );
}

void	trap_Args( char *buffer, int bufferLength ) {
	Q_syscall( CG_ARGS, buffer, bufferLength );
}

int		trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode ) {
	return Q_syscall( CG_FS_FOPENFILE, qpath, f, mode );
}

void	trap_FS_Read( void *buffer, int len, fileHandle_t f ) {
	Q_syscall( CG_FS_READ, buffer, len, f );
}

void	trap_FS_Write( const void *buffer, int len, fileHandle_t f ) {
	Q_syscall( CG_FS_WRITE, buffer, len, f );
}

void	trap_FS_FCloseFile( fileHandle_t f ) {
	Q_syscall( CG_FS_FCLOSEFILE, f );
}

int trap_FS_GetFileList(  const char *path, const char *extension, char *listbuf, int bufsize ) {
	return Q_syscall( CG_FS_GETFILELIST, path, extension, listbuf, bufsize );
}

void	trap_SendConsoleCommand( const char *text ) {
	Q_syscall( CG_SENDCONSOLECOMMAND, text );
}

void	trap_AddCommand( const char *cmdName ) {
	Q_syscall( CG_ADDCOMMAND, cmdName );
}

void	trap_RemoveCommand( const char *cmdName ) {
	Q_syscall( CG_REMOVECOMMAND, cmdName );
}

void	trap_SendClientCommand( const char *s ) {
	Q_syscall( CG_SENDCLIENTCOMMAND, s );
}

void	trap_UpdateScreen( void ) {
	Q_syscall( CG_UPDATESCREEN );
}

void	trap_CM_LoadMap( const char *mapname, qboolean SubBSP ) {
	Q_syscall( CG_CM_LOADMAP, mapname, SubBSP );
}

int		trap_CM_NumInlineModels( void ) {
	return Q_syscall( CG_CM_NUMINLINEMODELS );
}

clipHandle_t trap_CM_InlineModel( int index ) {
	return Q_syscall( CG_CM_INLINEMODEL, index );
}

clipHandle_t trap_CM_TempBoxModel( const vec3_t mins, const vec3_t maxs ) {
	return Q_syscall( CG_CM_TEMPBOXMODEL, mins, maxs );
}

clipHandle_t trap_CM_TempCapsuleModel( const vec3_t mins, const vec3_t maxs ) {
	return Q_syscall( CG_CM_TEMPCAPSULEMODEL, mins, maxs );
}

int		trap_CM_PointContents( const vec3_t p, clipHandle_t model ) {
	return Q_syscall( CG_CM_POINTCONTENTS, p, model );
}

int		trap_CM_TransformedPointContents( const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles ) {
	return Q_syscall( CG_CM_TRANSFORMEDPOINTCONTENTS, p, model, origin, angles );
}

void	trap_CM_BoxTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask ) {
	Q_syscall( CG_CM_BOXTRACE, results, start, end, mins, maxs, model, brushmask );
}

void	trap_CM_CapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask ) {
	Q_syscall( CG_CM_CAPSULETRACE, results, start, end, mins, maxs, model, brushmask );
}

void	trap_CM_TransformedBoxTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask,
						  const vec3_t origin, const vec3_t angles ) {
	Q_syscall( CG_CM_TRANSFORMEDBOXTRACE, results, start, end, mins, maxs, model, brushmask, origin, angles );
}

void	trap_CM_TransformedCapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask,
						  const vec3_t origin, const vec3_t angles ) {
	Q_syscall( CG_CM_TRANSFORMEDCAPSULETRACE, results, start, end, mins, maxs, model, brushmask, origin, angles );
}

int		trap_CM_MarkFragments( int numPoints, const vec3_t *points, 
				const vec3_t projection,
				int maxPoints, vec3_t pointBuffer,
				int maxFragments, markFragment_t *fragmentBuffer ) {
	return Q_syscall( CG_CM_MARKFRAGMENTS, numPoints, points, projection, maxPoints, pointBuffer, maxFragments, fragmentBuffer );
}

int trap_S_GetVoiceVolume( int entityNum ) {
	if (CG_MultiSpecActive()) return 0;
	return Q_syscall( CG_S_GETVOICEVOLUME, entityNum );
}

void	trap_S_StopSound( int entityNum, int entchannel, sfxHandle_t sfx ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_MUTESOUND, entityNum, entchannel, sfx );
}

void	trap_S_StartSound( vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_STARTSOUND, origin, entityNum, entchannel, sfx );
}

void	trap_S_StartLocalSound( sfxHandle_t sfx, int channelNum ) {
	if (CG_MultiSpecActive()) return;
	//announcer is always hearable, rite?
	if ( channelNum == CHAN_ANNOUNCER || channelNum == CHAN_LOCAL_SOUND )
		Q_syscall( CG_S_STARTSOUND, 0, ENTITYNUM_NONE, channelNum, sfx );
	else
		Q_syscall( CG_S_STARTLOCALSOUND, sfx, channelNum );
}

void	trap_S_ClearLoopingSounds(void) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_CLEARLOOPINGSOUNDS );
}

void	trap_S_AddLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_ADDLOOPINGSOUND, entityNum, origin, velocity, sfx );
}

void	trap_S_UpdateEntityPosition( int entityNum, const vec3_t origin ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_UPDATEENTITYPOSITION, entityNum, origin );
}

void	trap_S_AddRealLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_ADDREALLOOPINGSOUND, entityNum, origin, velocity, sfx );
}

void	trap_S_StopLoopingSound( int entityNum ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_STOPLOOPINGSOUND, entityNum );
}

void	trap_S_Respatialize( int entityNum, const vec3_t origin, vec3_t axis[3], int inwater ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_RESPATIALIZE, entityNum, origin, axis, inwater );
}

void trap_S_ShutUp(qboolean shutUpFactor)
{
	if (CG_MultiSpecActive()) return;
	Q_syscall(CG_S_SHUTUP, shutUpFactor);
}

sfxHandle_t	trap_S_RegisterSound( const char *sample ) {
	if (CG_MultiSpecActive()) return 0;
	return Q_syscall( CG_S_REGISTERSOUND, sample );
}

void	trap_S_StartBackgroundTrack( const char *intro, const char *loop, qboolean bReturnWithoutStarting  ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_STARTBACKGROUNDTRACK, intro, loop, bReturnWithoutStarting  );
}

void trap_S_UpdateAmbientSet( const char *name, vec3_t origin )
{
	if (CG_MultiSpecActive()) return;
	Q_syscall(CG_S_UPDATEAMBIENTSET, name, origin);
}

void trap_AS_ParseSets( void )
{
	Q_syscall(CG_AS_PARSESETS);
}

void trap_AS_AddPrecacheEntry( const char *name )
{
	Q_syscall(CG_AS_ADDPRECACHEENTRY, name);
}

int trap_S_AddLocalSet( const char *name, vec3_t listener_origin, vec3_t origin, int entID, int time )
{
	if (CG_MultiSpecActive()) return time;
	return Q_syscall(CG_S_ADDLOCALSET, name, listener_origin, origin, entID, time);
}

sfxHandle_t trap_AS_GetBModelSound( const char *name, int stage )
{
	return Q_syscall(CG_AS_GETBMODELSOUND, name, stage);
}

void	trap_R_LoadWorldMap( const char *mapname ) {
	Q_syscall( CG_R_LOADWORLDMAP, mapname );
}

qhandle_t trap_R_RegisterModel( const char *name ) {
	return Q_syscall( CG_R_REGISTERMODEL, name );
}

qhandle_t trap_R_RegisterSkin( const char *name ) {
	return Q_syscall( CG_R_REGISTERSKIN, name );
}

qhandle_t trap_R_RegisterShader( const char *name ) {
	return Q_syscall( CG_R_REGISTERSHADER, name );
}

qhandle_t trap_R_RegisterShaderNoMip( const char *name ) {
	return Q_syscall( CG_R_REGISTERSHADERNOMIP, name );
}

qhandle_t trap_R_RegisterFont( const char *fontName )
{
	return Q_syscall( CG_R_REGISTERFONT, fontName);
}

int	trap_R_Font_StrLenPixels(const char *text, const int iFontIndex, const float scale)
{
//	float x = 0.0f, y = 0.0f;
	//Raz: HACK! RE_Font_TtrLenPixels only works correctly with 1.0f scale
	float width = (float)Q_syscall( CG_R_FONT_STRLENPIXELS, text, iFontIndex, PASSFLOAT(1.0f));
//	CG_MultiSpecAdjustString(&x, &y, &scale);
	return width * scale;
}

int trap_R_Font_StrLenChars(const char *text)
{
	return Q_syscall( CG_R_FONT_STRLENCHARS, text);
}

int trap_R_Font_HeightPixels(const int iFontIndex, const float scale)
{
	float x = 0.0f, y = 0.0f;
	CG_MultiSpecAdjustString(&x, &y, &scale);
	return Q_syscall( CG_R_FONT_STRHEIGHTPIXELS, iFontIndex, PASSFLOAT(scale));
}

void trap_R_Font_DrawString(float ox, float oy, const char *text, const float *rgba, const int setIndex, int iCharLimit, const float scale)
{
	CG_MultiSpecAdjustString(&ox, &oy, &scale);
	Q_syscall( CG_R_FONT_DRAWSTRING, PASSFLOAT(ox), PASSFLOAT(oy), text, rgba, setIndex, iCharLimit, PASSFLOAT(scale));
}

qboolean trap_Language_IsAsian(void)
{
	return Q_syscall( CG_LANGUAGE_ISASIAN );
}

qboolean trap_Language_UsesSpaces(void)
{
	return Q_syscall( CG_LANGUAGE_USESSPACES );
}

unsigned int trap_AnyLanguage_ReadCharFromString( const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation/* = NULL*/ )
{
	return Q_syscall( CG_ANYLANGUAGE_READCHARFROMSTRING, psText, piAdvanceCount, pbIsTrailingPunctuation);
}

void	trap_R_ClearScene( void ) {
	Q_syscall( CG_R_CLEARSCENE );
}

void trap_R_ClearDecals ( void )
{
	Q_syscall ( CG_R_CLEARDECALS );
}

void	trap_R_AddRefEntityToScene( const refEntity_t *re ) {
	Q_syscall( CG_R_ADDREFENTITYTOSCENE, re );
}

void	trap_R_AddPolyToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts ) {
	Q_syscall( CG_R_ADDPOLYTOSCENE, hShader, numVerts, verts );
}

void	trap_R_AddPolysToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts, int num ) {
	Q_syscall( CG_R_ADDPOLYSTOSCENE, hShader, numVerts, verts, num );
}

void trap_R_AddDecalToScene ( qhandle_t shader, const vec3_t origin, const vec3_t dir, float orientation, float r, float g, float b, float a, qboolean alphaFade, float radius, qboolean temporary )
{
	Q_syscall( CG_R_ADDDECALTOSCENE, shader, origin, dir, PASSFLOAT(orientation), PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b), PASSFLOAT(a), alphaFade, PASSFLOAT(radius), temporary );
}

int		trap_R_LightForPoint( vec3_t point, vec3_t ambientLight, vec3_t directedLight, vec3_t lightDir ) {
	return Q_syscall( CG_R_LIGHTFORPOINT, point, ambientLight, directedLight, lightDir );
}

void	trap_R_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b ) {
	Q_syscall( CG_R_ADDLIGHTTOSCENE, org, PASSFLOAT(intensity), PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b) );
}

void	trap_R_AddAdditiveLightToScene( const vec3_t org, float intensity, float r, float g, float b ) {
	Q_syscall( CG_R_ADDADDITIVELIGHTTOSCENE, org, PASSFLOAT(intensity), PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b) );
}

void	trap_R_RenderScene( const refdef_t *fd ) {
	Q_syscall( CG_R_RENDERSCENE, fd );
}

void	trap_R_SetColor( const float *rgba ) {
	Q_syscall( CG_R_SETCOLOR, rgba );
}

void	trap_R_DrawStretchPic( float x, float y, float w, float h, 
							   float s1, float t1, float s2, float t2, qhandle_t hShader ) {
	CG_MultiSpecAdjust2D(&x, &y, &w, &h);
	Q_syscall( CG_R_DRAWSTRETCHPIC, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1), PASSFLOAT(s2), PASSFLOAT(t2), hShader );
}

void	trap_R_ModelBounds( clipHandle_t model, vec3_t mins, vec3_t maxs ) {
	Q_syscall( CG_R_MODELBOUNDS, model, mins, maxs );
}

int		trap_R_LerpTag( orientation_t *tag, clipHandle_t mod, int startFrame, int endFrame, 
					   float frac, const char *tagName ) {
	return Q_syscall( CG_R_LERPTAG, tag, mod, startFrame, endFrame, PASSFLOAT(frac), tagName );
}

void	trap_R_DrawRotatePic( float x, float y, float w, float h, 
				   float s1, float t1, float s2, float t2,float a, qhandle_t hShader ) 
{
	CG_MultiSpecAdjust2D(&x, &y, &w, &h);
	Q_syscall( CG_R_DRAWROTATEPIC, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1), PASSFLOAT(s2), PASSFLOAT(t2), PASSFLOAT(a), hShader );
}

void	trap_R_DrawRotatePic2( float x, float y, float w, float h, 
				   float s1, float t1, float s2, float t2,float a, qhandle_t hShader ) 
{
	CG_MultiSpecAdjust2D(&x, &y, &w, &h);
	Q_syscall( CG_R_DRAWROTATEPIC2, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1), PASSFLOAT(s2), PASSFLOAT(t2), PASSFLOAT(a), hShader );
}

//linear fogging, with settable range -rww
void trap_R_SetRangeFog(float range)
{
	Q_syscall(CG_R_SETRANGEFOG, PASSFLOAT(range));
}

//set some properties for the draw layer for my refractive effect (here primarily for mod authors) -rww
void trap_R_SetRefractProp(float alpha, float stretch, qboolean prepost, qboolean negate)
{
	Q_syscall(CG_R_SETREFRACTIONPROP, PASSFLOAT(alpha), PASSFLOAT(stretch), prepost, negate);
}

void	trap_R_RemapShader( const char *oldShader, const char *newShader, const char *timeOffset ) 
{
	Q_syscall( CG_R_REMAP_SHADER, oldShader, newShader, timeOffset );
}

void	trap_R_GetLightStyle(int style, color4ub_t color)
{
	Q_syscall( CG_R_GET_LIGHT_STYLE, style, color );
}

void	trap_R_SetLightStyle(int style, int color)
{
	Q_syscall( CG_R_SET_LIGHT_STYLE, style, color );
}

void	trap_R_GetBModelVerts(int bmodelIndex, vec3_t *verts, vec3_t normal )
{
	Q_syscall( CG_R_GET_BMODEL_VERTS, bmodelIndex, verts, normal );
}

void trap_R_GetDistanceCull(float *f)
{
	Q_syscall(CG_R_GETDISTANCECULL, f);
}

//get screen resolution -rww
void trap_R_GetRealRes(int *w, int *h)
{
	Q_syscall( CG_R_GETREALRES, w, h );
}


//automap elevation setting -rww
void trap_R_AutomapElevAdj(float newHeight)
{
	Q_syscall( CG_R_AUTOMAPELEVADJ, PASSFLOAT(newHeight) );
}

//initialize automap -rww
qboolean trap_R_InitWireframeAutomap(void)
{
	return Q_syscall( CG_R_INITWIREFRAMEAUTO );
}

void	trap_FX_AddLine( const vec3_t start, const vec3_t end, float size1, float size2, float sizeParm,
									float alpha1, float alpha2, float alphaParm,
									const vec3_t sRGB, const vec3_t eRGB, float rgbParm,
									int killTime, qhandle_t shader, int flags)
{
	Q_syscall( CG_FX_ADDLINE, start, end, PASSFLOAT(size1), PASSFLOAT(size2), PASSFLOAT(sizeParm),
									PASSFLOAT(alpha1), PASSFLOAT(alpha2), PASSFLOAT(alphaParm),
									sRGB, eRGB, PASSFLOAT(rgbParm),
									killTime, shader, flags);
}

void		trap_GetGlconfig( glconfig_t *glconfig ) {
	Q_syscall( CG_GETGLCONFIG, glconfig );
}

void		trap_GetGameState( gameState_t *gamestate ) {
	Q_syscall( CG_GETGAMESTATE, gamestate );
}

void		trap_GetCurrentSnapshotNumber( int *snapshotNumber, int *serverTime ) {
	Q_syscall( CG_GETCURRENTSNAPSHOTNUMBER, snapshotNumber, serverTime );
}

qboolean	trap_GetSnapshot( int snapshotNumber, snapshot_t *snapshot ) {
	return Q_syscall( CG_GETSNAPSHOT, snapshotNumber, snapshot );
}

qboolean	trap_GetDefaultState(int entityIndex, entityState_t *state )
{ //rwwRMG - added [NEWTRAP]
	return Q_syscall( CG_GETDEFAULTSTATE, entityIndex, state );
}

qboolean	trap_GetServerCommand( int serverCommandNumber ) {
	return Q_syscall( CG_GETSERVERCOMMAND, serverCommandNumber );
}

int			trap_GetCurrentCmdNumber( void ) {
	return Q_syscall( CG_GETCURRENTCMDNUMBER );
}

qboolean	trap_GetUserCmd( int cmdNumber, usercmd_t *ucmd ) {
	return Q_syscall( CG_GETUSERCMD, cmdNumber, ucmd );
}

void		trap_SetUserCmdValue( int stateValue, float sensitivityScale, float mPitchOverride, float mYawOverride, float mSensitivityOverride, int fpSel, int invenSel, qboolean fighterControls ) {
	Q_syscall( CG_SETUSERCMDVALUE, stateValue, PASSFLOAT(sensitivityScale), PASSFLOAT(mPitchOverride), PASSFLOAT(mYawOverride), PASSFLOAT(mSensitivityOverride), fpSel, invenSel, fighterControls );
}

void trap_SetClientForceAngle(int time, vec3_t angle)
{
	Q_syscall( CG_SETCLIENTFORCEANGLE, time, angle );
}

void trap_SetClientTurnExtent(float turnAdd, float turnSub, int turnTime)
{
	Q_syscall( CG_SETCLIENTTURNEXTENT, PASSFLOAT(turnAdd), PASSFLOAT(turnSub), turnTime );
}

void trap_OpenUIMenu(int menuID)
{
	Q_syscall( CG_OPENUIMENU, menuID );
}

void		testPrintInt( char *string, int i ) {
	Q_syscall( CG_TESTPRINTINT, string, i );
}

void		testPrintFloat( char *string, float f ) {
	Q_syscall( CG_TESTPRINTFLOAT, string, PASSFLOAT(f) );
}

int trap_MemoryRemaining( void ) {
	return Q_syscall( CG_MEMORY_REMAINING );
}

qboolean trap_Key_IsDown( int keynum ) {
	return Q_syscall( CG_KEY_ISDOWN, keynum );
}

int trap_Key_GetCatcher( void ) {
	return Q_syscall( CG_KEY_GETCATCHER );
}

void trap_Key_SetCatcher( int catcher ) {
	Q_syscall( CG_KEY_SETCATCHER, catcher );
}

int trap_Key_GetKey( const char *binding ) {
	return Q_syscall( CG_KEY_GETKEY, binding );
}

qboolean trap_Key_GetOverstrikeMode( void ) {
	return Q_syscall( CG_KEY_GETOVERSTRIKEMODE );
}

void trap_Key_SetOverstrikeMode( qboolean state ) {
	Q_syscall( CG_KEY_SETOVERSTRIKEMODE, state );
}

int trap_PC_AddGlobalDefine( char *define ) {
	return Q_syscall( CG_PC_ADD_GLOBAL_DEFINE, define );
}

int trap_PC_LoadSource( const char *filename ) {
	return Q_syscall( CG_PC_LOAD_SOURCE, filename );
}

int trap_PC_FreeSource( int handle ) {
	return Q_syscall( CG_PC_FREE_SOURCE, handle );
}

int trap_PC_ReadToken( int handle, pc_token_t *pc_token ) {
	return Q_syscall( CG_PC_READ_TOKEN, handle, pc_token );
}

int trap_PC_SourceFileAndLine( int handle, char *filename, int *line ) {
	return Q_syscall( CG_PC_SOURCE_FILE_AND_LINE, handle, filename, line );
}

int trap_PC_LoadGlobalDefines ( const char* filename )
{
	return Q_syscall ( CG_PC_LOAD_GLOBAL_DEFINES, filename );
}

void trap_PC_RemoveAllGlobalDefines ( void )
{
	Q_syscall ( CG_PC_REMOVE_ALL_GLOBAL_DEFINES );
}

void	trap_S_StopBackgroundTrack( void ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_STOPBACKGROUNDTRACK );
}

int trap_RealTime(qtime_t *qtime) {
	return Q_syscall( CG_REAL_TIME, qtime );
}

void trap_SnapVector( float *v ) {
	Q_syscall( CG_SNAPVECTOR, v );
}

// this returns a handle.  arg0 is the name in the format "idlogo.roq", set arg1 to NULL, alteredstates to qfalse (do not alter gamestate)
int trap_CIN_PlayCinematic( const char *arg0, int xpos, int ypos, int width, int height, int bits) {
  return Q_syscall(CG_CIN_PLAYCINEMATIC, arg0, xpos, ypos, width, height, bits);
}
 
// stops playing the cinematic and ends it.  should always return FMV_EOF
// cinematics must be stopped in reverse order of when they are started
e_status trap_CIN_StopCinematic(int handle) {
  return Q_syscall(CG_CIN_STOPCINEMATIC, handle);
}


// will run a frame of the cinematic but will not draw it.  Will return FMV_EOF if the end of the cinematic has been reached.
e_status trap_CIN_RunCinematic (int handle) {
  return Q_syscall(CG_CIN_RUNCINEMATIC, handle);
}
 

// draws the current frame
void trap_CIN_DrawCinematic (int handle) {
  Q_syscall(CG_CIN_DRAWCINEMATIC, handle);
}
 

// allows you to resize the animation dynamically
void trap_CIN_SetExtents (int handle, int x, int y, int w, int h) {
  Q_syscall(CG_CIN_SETEXTENTS, handle, x, y, w, h);
}

qboolean trap_GetEntityToken( char *buffer, int bufferSize ) {
	return Q_syscall( CG_GET_ENTITY_TOKEN, buffer, bufferSize );
}

qboolean trap_R_inPVS( const vec3_t p1, const vec3_t p2, byte *mask ) {
	return Q_syscall( CG_R_INPVS, p1, p2, mask );
}


int	trap_FX_RegisterEffect(const char *file)
{
	return Q_syscall( CG_FX_REGISTER_EFFECT, file);
}

void trap_FX_PlayEffect( const char *file, vec3_t org, vec3_t fwd, int vol, int rad )
{
	Q_syscall( CG_FX_PLAY_EFFECT, file, org, fwd, vol, rad);
}

void trap_FX_PlayEntityEffect( const char *file, vec3_t org, 
						vec3_t axis[3], const int boltInfo, const int entNum, int vol, int rad )
{
	Q_syscall( CG_FX_PLAY_ENTITY_EFFECT, file, org, axis, boltInfo, entNum, vol, rad );
}

void trap_FX_PlayEffectID( int id, vec3_t org, vec3_t fwd, int vol, int rad )
{
	if (id == cgs.effects.rocketShotEffect				||
		id == cgs.effects.repeaterAltProjectileEffect	||
		id == cgs.effects.flechetteAltShotEffect 		||
		id == cgs.effects.tripmineLaserFX 				||
		id == cgs.effects.tripmineGlowFX 				||
		id == cgs.effects.itemCone						||
		id == cgs.effects.mSpawn						||
		id == cgs.effects.mSparks						||
		id == cgs.effects.bryarPowerupShotEffect		||
		id == cgs.effects.bryarShotEffect				||
		id == cgs.effects.turretShotEffect				||
		id == cgs.effects.blasterShotEffect				||
		id == cgs.effects.bowcasterShotEffect			||
		id == cgs.effects.repeaterProjectileEffect		||
		id == cgs.effects.repeaterAltProjectileEffect	||
		id == cgs.effects.demp2ProjectileEffect			||
		id == cgs.effects.concussionShotEffect			||
		id == cgs.effects.flechetteShotEffect			||
		id == cgs.effects.mBobaJet						||
		id == cgs.effects.saberFizz) {
			if (fx_vfps.integer <= 0)
				fx_vfps.integer = 1;
			if (fxT > cg.time)
				fxT = cg.time;
			if (doFX || cg.time - fxT >= (1000.0f / (float)fx_vfps.integer)) {
				doFX = qtrue;
				fxT = cg.time;
			} else {
				doFX = qfalse;
				return;
			}
	}

/*	if (id == cgs.effects.itemCone && !(cg.frametime > 0
		&& ((cg.frametime < 17 && fmod((float)cg.time, 17.0f) <= cg.frametime)
		|| cg.frametime >= 17)))
		return;*/

	Q_syscall( CG_FX_PLAY_EFFECT_ID, id, org, fwd, vol, rad );
}

void trap_FX_PlayEffectIDFix( int id, vec3_t org, vec3_t fwd, int vol, int rad )
{
	if (fx_vfps.integer <= 0)
		fx_vfps.integer = 1;
	if (fxT > cg.time)
		fxT = cg.time;
	if (doFX || cg.time - fxT >= 1000 / fx_vfps.integer) {
		doFX = qtrue;
		fxT = cg.time;
	} else {
		doFX = qfalse;
		return;
	}

	Q_syscall( CG_FX_PLAY_EFFECT_ID, id, org, fwd, vol, rad );
}

void trap_FX_PlayPortalEffectID( int id, vec3_t org, vec3_t fwd, int vol, int rad )
{
	Q_syscall( CG_FX_PLAY_PORTAL_EFFECT_ID, id, org, fwd);
}

void trap_FX_PlayEntityEffectID( int id, vec3_t org, 
						vec3_t axis[3], const int boltInfo, const int entNum, int vol, int rad )
{
	if (id == cgs.effects.forceLightning			||
		id == cgs.effects.forceLightningWide		||
		id == cgs.effects.forceDrainWide			||
		id == cgs.effects.forceDrain				||
		id == cgs.effects.mForceConfustionOld) {

		if (fx_vfps.integer <= 0)
			fx_vfps.integer = 1;
		if (fxT > cg.time)
			fxT = cg.time;
		if (doFX || cg.time - fxT >= 1000 / fx_vfps.integer) {
			doFX = qtrue;
			fxT = cg.time;
		} else {
			doFX = qfalse;
			return;
		}

		if (id != cgs.effects.mForceConfustionOld) {
			if (!(cg.frametime > 0
				&& ((cg.frametime < 7 && fmod((double)cg.time, 7.0) <= (double)cg.frametime)
				|| cg.frametime >= 7)))
				return;
		} else if (!(cg.frametime > 0
			&& ((cg.frametime < 17 && fmod((float)cg.time, 17.0f) <= cg.frametime)
			|| cg.frametime >= 17)))
			return;
	}
	Q_syscall( CG_FX_PLAY_ENTITY_EFFECT_ID, id, org, axis, boltInfo, entNum, vol, rad );
}

void trap_FX_PlayBoltedEffectID( int id, vec3_t org, 
						void *ghoul2, const int boltNum, const int entNum, const int modelNum, int iLooptime, qboolean isRelative )
{
	Q_syscall( CG_FX_PLAY_BOLTED_EFFECT_ID, id, org, ghoul2, boltNum, entNum, modelNum, iLooptime, isRelative );
}

void trap_FX_AddScheduledEffects( qboolean skyPortal )
{
	Q_syscall( CG_FX_ADD_SCHEDULED_EFFECTS, skyPortal );
}

void trap_FX_Draw2DEffects ( float screenXScale, float screenYScale )
{
	Q_syscall( CG_FX_DRAW_2D_EFFECTS, PASSFLOAT(screenXScale), PASSFLOAT(screenYScale) );
}	

int	trap_FX_InitSystem( refdef_t* refdef )
{
	return Q_syscall( CG_FX_INIT_SYSTEM, refdef );
}

void trap_FX_SetRefDef( refdef_t* refdef )
{
	Q_syscall( CG_FX_SET_REFDEF, refdef );
}

qboolean trap_FX_FreeSystem( void )
{
	return Q_syscall( CG_FX_FREE_SYSTEM );
}

void trap_FX_Reset ( void )
{
	Q_syscall ( CG_FX_RESET );
}

void trap_FX_AdjustTime( int time, float frametime, float timeFraction )
{
	Q_syscall( CG_FX_ADJUST_TIME, time, PASSFLOAT(frametime), PASSFLOAT(timeFraction) );
}


void trap_FX_AddPoly( addpolyArgStruct_t *p )
{
	Q_syscall( CG_FX_ADDPOLY, p );
}

void trap_FX_AddBezier( addbezierArgStruct_t *p )
{
	Q_syscall( CG_FX_ADDBEZIER, p );
}

void trap_FX_AddPrimitive( effectTrailArgStruct_t *p )
{
	Q_syscall( CG_FX_ADDPRIMITIVE, p );
}

void trap_FX_AddSprite( addspriteArgStruct_t *p )
{
	if( p->shader == cgs.media.bryarFrontFlash			||
		p->shader == cgs.media.greenFrontFlash			||
		p->shader == cgs.media.lightningFlash			||
		p->shader == cgs.media.yellowDroppedSaberShader	||
		p->shader == cgs.media.redSaberGlowShader		||
		p->shader == cgs.media.greenSaberGlowShader		||
		p->shader == cgs.media.blueSaberGlowShader ) {
			if (fx_vfps.integer <= 0)
				fx_vfps.integer = 1;
			if (fxT > cg.time)
				fxT = cg.time;
			if( doFX || cg.time - fxT >= 1000 / fx_vfps.integer )
			{
				doFX = qtrue;
				fxT = cg.time;
			}
			else 
			{
				doFX = qfalse;
				//syscall( CG_FX_ADJUST_TIME, time );
				return;
			}
	}
	Q_syscall( CG_FX_ADDSPRITE, p );
}

void trap_FX_AddElectricity( addElectricityArgStruct_t *p )
{
	Q_syscall( CG_FX_ADDELECTRICITY, p );
}

//void trap_SP_Print(const unsigned ID, byte *Data)
//{
//	Q_syscall( CG_SP_PRINT, ID, Data);
//}

int trap_SP_GetStringTextString(const char *text, char *buffer, int bufferLength)
{
	return Q_syscall( CG_SP_GETSTRINGTEXTSTRING, text, buffer, bufferLength );
}

qboolean trap_ROFF_Clean( void ) 
{
	return Q_syscall( CG_ROFF_CLEAN );
}

void trap_ROFF_UpdateEntities( void ) 
{
	Q_syscall( CG_ROFF_UPDATE_ENTITIES );
}

int trap_ROFF_Cache( char *file ) 
{
	return Q_syscall( CG_ROFF_CACHE, file );
}

qboolean trap_ROFF_Play( int entID, int roffID, qboolean doTranslation ) 
{
	return Q_syscall( CG_ROFF_PLAY, entID, roffID, doTranslation );
}

qboolean trap_ROFF_Purge_Ent( int entID ) 
{
	return Q_syscall( CG_ROFF_PURGE_ENT, entID );
}


//rww - dynamic vm memory allocation!
void trap_TrueMalloc(void **ptr, int size)
{
	Q_syscall(CG_TRUEMALLOC, ptr, size);
}

void trap_TrueFree(void **ptr)
{
	Q_syscall(CG_TRUEFREE, ptr);
}

/*
Ghoul2 Insert Start
*/
// CG Specific API calls
void trap_G2_ListModelSurfaces(void *ghlInfo)
{
	Q_syscall( CG_G2_LISTSURFACES, ghlInfo);
}

void trap_G2_ListModelBones(void *ghlInfo, int frame)
{
	Q_syscall( CG_G2_LISTBONES, ghlInfo, frame);
}

void trap_G2_SetGhoul2ModelIndexes(void *ghoul2, qhandle_t *modelList, qhandle_t *skinList)
{
	Q_syscall( CG_G2_SETMODELS, ghoul2, modelList, skinList);
}

qboolean trap_G2_HaveWeGhoul2Models(void *ghoul2)
{
	return (qboolean)(Q_syscall(CG_G2_HAVEWEGHOULMODELS, ghoul2));
}

qboolean trap_G2API_GetBoltMatrix(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix,
								const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale)
{
	return (qboolean)(Q_syscall(CG_G2_GETBOLT, ghoul2, modelIndex, boltIndex, matrix, angles, position, frameNum, modelList, scale));
}

qboolean trap_G2API_GetBoltMatrix_NoReconstruct(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix,
								const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale)
{ //Same as above but force it to not reconstruct the skeleton before getting the bolt position
	return (qboolean)(Q_syscall(CG_G2_GETBOLT_NOREC, ghoul2, modelIndex, boltIndex, matrix, angles, position, frameNum, modelList, scale));
}

qboolean trap_G2API_GetBoltMatrix_NoRecNoRot(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix,
								const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale)
{ //Same as above but force it to not reconstruct the skeleton before getting the bolt position
	return (qboolean)(Q_syscall(CG_G2_GETBOLT_NOREC_NOROT, ghoul2, modelIndex, boltIndex, matrix, angles, position, frameNum, modelList, scale));
}

int trap_G2API_InitGhoul2Model(void **ghoul2Ptr, const char *fileName, int modelIndex, qhandle_t customSkin,
						  qhandle_t customShader, int modelFlags, int lodBias)
{
	return Q_syscall(CG_G2_INITGHOUL2MODEL, ghoul2Ptr, fileName, modelIndex, customSkin, customShader, modelFlags, lodBias);
}

qboolean trap_G2API_SetSkin(void *ghoul2, int modelIndex, qhandle_t customSkin, qhandle_t renderSkin)
{
	return Q_syscall(CG_G2_SETSKIN, ghoul2, modelIndex, customSkin, renderSkin);
}

void trap_G2API_CollisionDetect ( 
	CollisionRecord_t *collRecMap, 
	void* ghoul2, 
	const vec3_t angles, 
	const vec3_t position,
	int frameNumber, 
	int entNum, 
	const vec3_t rayStart, 
	const vec3_t rayEnd, 
	const vec3_t scale, 
	int traceFlags, 
	int useLod,
	float fRadius
	)
{
	Q_syscall ( CG_G2_COLLISIONDETECT, collRecMap, ghoul2, angles, position, frameNumber, entNum, rayStart, rayEnd, scale, traceFlags, useLod, PASSFLOAT(fRadius) );
}

void trap_G2API_CollisionDetectCache ( 
	CollisionRecord_t *collRecMap, 
	void* ghoul2, 
	const vec3_t angles, 
	const vec3_t position,
	int frameNumber, 
	int entNum, 
	const vec3_t rayStart, 
	const vec3_t rayEnd, 
	const vec3_t scale, 
	int traceFlags, 
	int useLod,
	float fRadius
	)
{
	Q_syscall ( CG_G2_COLLISIONDETECTCACHE, collRecMap, ghoul2, angles, position, frameNumber, entNum, rayStart, rayEnd, scale, traceFlags, useLod, PASSFLOAT(fRadius) );
}

void trap_G2API_CleanGhoul2Models(void **ghoul2Ptr)
{
	Q_syscall(CG_G2_CLEANMODELS, ghoul2Ptr);
}

qboolean trap_G2API_SetBoneAngles(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags,
								const int up, const int right, const int forward, qhandle_t *modelList,
								int blendTime , int currentTime )
{
	return (Q_syscall(CG_G2_ANGLEOVERRIDE, ghoul2, modelIndex, boneName, angles, flags, up, right, forward, modelList, blendTime, currentTime));
}

qboolean trap_G2API_SetBoneAnim(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame,
							  const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime )
{
	return Q_syscall(CG_G2_PLAYANIM, ghoul2, modelIndex, boneName, startFrame, endFrame, flags, PASSFLOAT(animSpeed), currentTime, PASSFLOAT(setFrame), blendTime);
}

qboolean trap_G2API_GetBoneAnim(void *ghoul2, const char *boneName, const int currentTime, float *currentFrame,
						   int *startFrame, int *endFrame, int *flags, float *animSpeed, int *modelList, const int modelIndex)
{
	return Q_syscall(CG_G2_GETBONEANIM, ghoul2, boneName, currentTime, currentFrame, startFrame, endFrame, flags, animSpeed, modelList, modelIndex);
}

qboolean trap_G2API_GetBoneFrame(void *ghoul2, const char *boneName, const int currentTime, float *currentFrame, int *modelList, const int modelIndex)
{
	return Q_syscall(CG_G2_GETBONEFRAME, ghoul2, boneName, currentTime, currentFrame, modelList, modelIndex);
}

void trap_G2API_GetGLAName(void *ghoul2, int modelIndex, char *fillBuf)
{
	Q_syscall(CG_G2_GETGLANAME, ghoul2, modelIndex, fillBuf);
}

int trap_G2API_CopyGhoul2Instance(void *g2From, void *g2To, int modelIndex)
{
	return Q_syscall(CG_G2_COPYGHOUL2INSTANCE, g2From, g2To, modelIndex);
}

void trap_G2API_CopySpecificGhoul2Model(void *g2From, int modelFrom, void *g2To, int modelTo)
{
	if (CG_MultiSpecActive()) return;
	Q_syscall(CG_G2_COPYSPECIFICGHOUL2MODEL, g2From, modelFrom, g2To, modelTo);
}

void trap_G2API_DuplicateGhoul2Instance(void *g2From, void **g2To)
{
	Q_syscall(CG_G2_DUPLICATEGHOUL2INSTANCE, g2From, g2To);
}

qboolean trap_G2API_HasGhoul2ModelOnIndex(void *ghlInfo, int modelIndex)
{
	return Q_syscall(CG_G2_HASGHOUL2MODELONINDEX, ghlInfo, modelIndex);
}

qboolean trap_G2API_RemoveGhoul2Model(void *ghlInfo, int modelIndex)
{
	if (CG_MultiSpecActive()) return qtrue;
	return Q_syscall(CG_G2_REMOVEGHOUL2MODEL, ghlInfo, modelIndex);
}

qboolean trap_G2API_SkinlessModel(void *ghlInfo, int modelIndex)
{
	return Q_syscall(CG_G2_SKINLESSMODEL, ghlInfo, modelIndex);
}

int trap_G2API_GetNumGoreMarks(void *ghlInfo, int modelIndex)
{
	return Q_syscall(CG_G2_GETNUMGOREMARKS, ghlInfo, modelIndex);
}

void trap_G2API_AddSkinGore(void *ghlInfo,SSkinGoreData *gore)
{
	Q_syscall(CG_G2_ADDSKINGORE, ghlInfo, gore);
}

void trap_G2API_ClearSkinGore ( void* ghlInfo )
{
	Q_syscall(CG_G2_CLEARSKINGORE, ghlInfo );
}

int trap_G2API_Ghoul2Size ( void* ghlInfo )
{
	return Q_syscall(CG_G2_SIZE, ghlInfo );
}

int	trap_G2API_AddBolt(void *ghoul2, int modelIndex, const char *boneName)
{
	return Q_syscall(CG_G2_ADDBOLT, ghoul2, modelIndex, boneName);
}

qboolean trap_G2API_AttachEnt(int *boltInfo, void *ghlInfoTo, int toBoltIndex, int entNum, int toModelNum)
{
	return Q_syscall(CG_G2_ATTACHENT, boltInfo, ghlInfoTo, toBoltIndex, entNum, toModelNum);
}

void trap_G2API_SetBoltInfo(void *ghoul2, int modelIndex, int boltInfo)
{
	Q_syscall(CG_G2_SETBOLTON, ghoul2, modelIndex, boltInfo);
}

qboolean trap_G2API_SetRootSurface(void *ghoul2, const int modelIndex, const char *surfaceName)
{
	return Q_syscall(CG_G2_SETROOTSURFACE, ghoul2, modelIndex, surfaceName);
}

qboolean trap_G2API_SetSurfaceOnOff(void *ghoul2, const char *surfaceName, const int flags)
{
	return Q_syscall(CG_G2_SETSURFACEONOFF, ghoul2, surfaceName, flags);
}

qboolean trap_G2API_SetNewOrigin(void *ghoul2, const int boltIndex)
{
	return Q_syscall(CG_G2_SETNEWORIGIN, ghoul2, boltIndex);
}

//check if a bone exists on skeleton without actually adding to the bone list -rww
qboolean trap_G2API_DoesBoneExist(void *ghoul2, int modelIndex, const char *boneName)
{
	return Q_syscall(CG_G2_DOESBONEEXIST, ghoul2, modelIndex, boneName);
}

int trap_G2API_GetSurfaceRenderStatus(void *ghoul2, const int modelIndex, const char *surfaceName)
{
	return Q_syscall(CG_G2_GETSURFACERENDERSTATUS, ghoul2, modelIndex, surfaceName);
}

int trap_G2API_GetTime(void)
{
	return Q_syscall(CG_G2_GETTIME);
}

void trap_G2API_SetTime(int time, int clock)
{
	Q_syscall(CG_G2_SETTIME, time, clock);
}

void trap_G2API_SetTimeFraction(float timeFraction)
{
	Q_syscall(CG_G2_SETTIMEFRACTION, PASSFLOAT(timeFraction));
}

//hack for smoothing during ugly situations. forgive me.
void trap_G2API_AbsurdSmoothing(void *ghoul2, qboolean status)
{
	Q_syscall(CG_G2_ABSURDSMOOTHING, ghoul2, status);
}

//rww - RAGDOLL_BEGIN
void trap_G2API_SetRagDoll(void *ghoul2, sharedRagDollParams_t *params)
{
	Q_syscall(CG_G2_SETRAGDOLL, ghoul2, params);
}

void trap_G2API_AnimateG2Models(void *ghoul2, int time, sharedRagDollUpdateParams_t *params)
{
	Q_syscall(CG_G2_ANIMATEG2MODELS, ghoul2, time, params);
}
//rww - RAGDOLL_END

//additional ragdoll options -rww
qboolean trap_G2API_RagPCJConstraint(void *ghoul2, const char *boneName, vec3_t min, vec3_t max) //override default pcj bonee constraints
{
	return Q_syscall(CG_G2_RAGPCJCONSTRAINT, ghoul2, boneName, min, max);
}

qboolean trap_G2API_RagPCJGradientSpeed(void *ghoul2, const char *boneName, const float speed) //override the default gradient movespeed for a pcj bone
{
	return Q_syscall(CG_G2_RAGPCJGRADIENTSPEED, ghoul2, boneName, PASSFLOAT(speed));
}

qboolean trap_G2API_RagEffectorGoal(void *ghoul2, const char *boneName, vec3_t pos) //override an effector bone's goal position (world coordinates)
{
	return Q_syscall(CG_G2_RAGEFFECTORGOAL, ghoul2, boneName, pos);
}

qboolean trap_G2API_GetRagBonePos(void *ghoul2, const char *boneName, vec3_t pos, vec3_t entAngles, vec3_t entPos, vec3_t entScale) //current position of said bone is put into pos (world coordinates)
{
	return Q_syscall(CG_G2_GETRAGBONEPOS, ghoul2, boneName, pos, entAngles, entPos, entScale);
}

qboolean trap_G2API_RagEffectorKick(void *ghoul2, const char *boneName, vec3_t velocity) //add velocity to a rag bone
{
	return Q_syscall(CG_G2_RAGEFFECTORKICK, ghoul2, boneName, velocity);
}

qboolean trap_G2API_RagForceSolve(void *ghoul2, qboolean force) //make sure we are actively performing solve/settle routines, if desired
{
	return Q_syscall(CG_G2_RAGFORCESOLVE, ghoul2, force);
}

qboolean trap_G2API_SetBoneIKState(void *ghoul2, int time, const char *boneName, int ikState, sharedSetBoneIKStateParams_t *params)
{
	return Q_syscall(CG_G2_SETBONEIKSTATE, ghoul2, time, boneName, ikState, params);
}

qboolean trap_G2API_IKMove(void *ghoul2, int time, sharedIKMoveParams_t *params)
{
	return Q_syscall(CG_G2_IKMOVE, ghoul2, time, params);
}

qboolean trap_G2API_RemoveBone(void *ghoul2, const char *boneName, int modelIndex)
{
	return Q_syscall(CG_G2_REMOVEBONE, ghoul2, boneName, modelIndex);
}

//rww - Stuff to allow association of ghoul2 instances to entity numbers.
//This way, on listen servers when both the client and server are doing
//ghoul2 operations, we can copy relevant data off the client instance
//directly onto the server instance and slash the transforms and whatnot
//right in half.
void trap_G2API_AttachInstanceToEntNum(void *ghoul2, int entityNum, qboolean server)
{
	Q_syscall(CG_G2_ATTACHINSTANCETOENTNUM, ghoul2, entityNum, server);
}

void trap_G2API_ClearAttachedInstance(int entityNum)
{
	Q_syscall(CG_G2_CLEARATTACHEDINSTANCE, entityNum);
}

void trap_G2API_CleanEntAttachments(void)
{
	Q_syscall(CG_G2_CLEANENTATTACHMENTS);
}

qboolean trap_G2API_OverrideServer(void *serverInstance)
{
	return Q_syscall(CG_G2_OVERRIDESERVER, serverInstance);
}

void trap_G2API_GetSurfaceName(void *ghoul2, int surfNumber, int modelIndex, char *fillBuf)
{
	Q_syscall(CG_G2_GETSURFACENAME, ghoul2, surfNumber, modelIndex, fillBuf);
}

void trap_CG_RegisterSharedMemory(char *memory)
{
	Q_syscall(CG_SET_SHARED_BUFFER, memory);
}

int	trap_CM_RegisterTerrain(const char *config)
{ //rwwRMG - added [NEWTRAP]
	return Q_syscall(CG_CM_REGISTER_TERRAIN, config);
}

void trap_RMG_Init(int terrainID, const char *terrainInfo)
{ //rwwRMG - added [NEWTRAP]
	Q_syscall(CG_RMG_INIT, terrainID, terrainInfo);
}

void trap_RE_InitRendererTerrain( const char *info )
{ //rwwRMG - added [NEWTRAP]
	Q_syscall(CG_RE_INIT_RENDERER_TERRAIN, info);
}

void trap_R_WeatherContentsOverride( int contents )
{ //rwwRMG - added [NEWTRAP]
	Q_syscall(CG_R_WEATHER_CONTENTS_OVERRIDE, contents);
}

void trap_R_WorldEffectCommand(const char *cmd)
{
	Q_syscall(CG_R_WORLDEFFECTCOMMAND, cmd);
}

void trap_WE_AddWeatherZone( const vec3_t mins, const vec3_t maxs )
{
	Q_syscall( CG_WE_ADDWEATHERZONE, mins, maxs );
}

/*
Ghoul2 Insert End
*/



/* New MME Syscalls */

void trap_MME_Capture( const char *baseName, float fps, float focus, float radius ) {
	Q_syscall( CG_MME_CAPTURE, baseName, PASSFLOAT(fps), PASSFLOAT( focus ), PASSFLOAT( radius ) );
}
void trap_MME_BlurInfo( int* total, int* index ) {
	Q_syscall( CG_MME_BLURINFO, total, index );
}
int trap_MME_SeekTime( int seekTime ) {
	return Q_syscall( CG_MME_SEEKTIME, seekTime );
}
void trap_MME_Music( const char *musicName, float time, float length ) {
	Q_syscall( CG_MME_MUSIC, musicName, PASSFLOAT(time), PASSFLOAT(length) );
}
void trap_MME_TimeFraction( float timeFraction ) {
	Q_syscall( CG_MME_TIMEFRACTION, PASSFLOAT(timeFraction) );
}
void trap_MME_ExtendedColors( colorTable_t cTable ) {
	Q_syscall( CG_MME_EXTENDEDCOLORS, (int)cTable );
}
void trap_MME_FontRatioFix( float ratio ) {
	Q_syscall( CG_MME_FONTRATIOFIX, PASSFLOAT(ratio) );
}
void trap_R_RandomSeed( int time, float timeFraction ) {
	Q_syscall( CG_R_RANDOMSEED, time, PASSFLOAT(timeFraction) );
}
void trap_FX_RandomSeed( int time, float timeFraction ) {
	Q_syscall( CG_FX_RANDOMSEED, time, PASSFLOAT(timeFraction) );
}
void trap_S_UpdateScale( float scale ) {
	if (CG_MultiSpecActive()) return;
	Q_syscall( CG_S_UPDATE_SCALE, PASSFLOAT(scale) );
}
void trap_CIN_AdjustTime( int time ) {
	Q_syscall( CG_CIN_ADJUST_TIME, time );
}
void trap_R_RotatePic2RatioFix( float ratio ) {
	Q_syscall( CG_R_ROTATEPIC2_RATIOFIX, PASSFLOAT(ratio) );
}
void trap_MME_VibrateFeedback( int time ) {
	Q_syscall( CG_MME_VIBRATEFEEDBACK, time );
}
float trap_MME_ProgressTime( void ) {
	floatint_t fi;
	fi.i = Q_syscall( CG_MME_PROGRESSTIME );
	return fi.f;
}
int trap_MME_DemoLength( void ) {
	return Q_syscall( CG_MME_DEMOLENGTH );
}
void trap_MME_RequestFeatures( void ) {
	Q_syscall( CG_MME_REQUESTFEATURES );
}
void trap_MME_ShowNotification( const char *message, const int flags ) {
	Q_syscall( CG_MME_NOTIFICATION, message, flags );
}