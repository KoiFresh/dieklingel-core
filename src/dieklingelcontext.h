#ifndef DIEKLINGELCONTEXT_H
#define DIEKLINGELCONTEXT_H

#include <QString>

/*
 *  dieklingelcontext.h
 *  Stellt alle contexte bereit, welche im DUP V1 enthalten sind
 */


namespace dieklingel
{
    // Last Context Update - 2021-03-11
    enum Context {
        Response,
        DeviceUpdate,
        ConnectionState,
        Movement,
        DisplayState,
        Log,
        Unlock,
        SecureUnlock,
        EnterPasscode,
        Call,
        Register,
        Script,
        Unknown
    };

    static QString ContextAsQString(Context context){
        switch (context) {
            case Response: return "response";
            case DeviceUpdate: return "device-update";
            case ConnectionState: return "connection-state";
            case Movement : return "movement";
            case DisplayState: return  "display-state";
            case Log: return "log";
            case Unlock: return  "unlock";
            case SecureUnlock: return "secure-unlock";
            case EnterPasscode : return "enter-passcode";
            case Call: return  "call";
            case Register: return  "register";
            case Script: return  "script";
            default: return "unknown";
        }
    }

    static Context QStringAsContext(QString context)
    {
        Context res = Unknown;
        for(int i = Response; i != Unknown; i++)
        {
            Context ctx = static_cast<Context>(i);
            if(ContextAsQString(ctx) == context)
            {
                res = ctx;
            }
        }
        return  res;
    }
}

#endif // DIEKLINGELCONTEXT_H
