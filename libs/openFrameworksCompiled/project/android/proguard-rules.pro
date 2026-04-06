# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

-dontoptimize
-dontshrink
-dontusemixedcaseclassnames
-dontskipnonpubliclibraryclasses
-dontpreverify
-verbose
#
#-optimizationpasses 5
#-dontusemixedcaseclassnames
#-dontskipnonpubliclibraryclasses
#-dontpreverify
#-verbose

-dontobfuscate

-keep public class * extends android.app.Activity
-keep public class * extends android.app.Application
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider
-keep public class * extends android.app.backup.BackupAgentHelper
-keep public class * extends android.preference.Preference


-keep class com.google.android.gms.games.leaderboard.** { *; }
-keep class com.google.android.gms.games.snapshot.** { *; }
-keep class com.google.android.gms.games.achievement.** { *; }
-keep class com.google.android.gms.games.event.** { *; }
-keep class com.google.android.gms.games.stats.** { *; }
-keep class com.google.android.gms.games.video.** { *; }
-keep class com.google.android.gms.games.* { *; }
#-keep class com.google.android.gms.common.api.ResultCallback { *; }
-keep class com.google.android.gms.signin.** { *; }
-keep class com.google.android.gms.dynamic.** { *; }
-keep class com.google.android.gms.dynamite.** { *; }
-keep class com.google.android.gms.tasks.** { *; }
-keep class com.google.android.gms.security.** { *; }
-keep class com.google.android.gms.base.** { *; }
-keep class com.google.android.gms.actions.** { *; }
-keep class com.google.games.bridge.** { *; }
#-keep class com.google.android.gms.common.ConnectionResult { *; }
#-keep class com.google.android.gms.common.GooglePlayServicesUtil { *; }
-keep class com.google.android.gms.common.api.** { *; }
#-keep class com.google.android.gms.common.data.DataBufferUtils { *; }
-keep class com.google.android.gms.games.quest.** { *; }
-keep class com.google.android.gms.nearby.** { *; }


-keepclasseswithmembernames class * {
    native <methods>;
}

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet);
}

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet, int);
}

# note that <methods> means any method
-keepclasseswithmembernames,includedescriptorclasses class * {
    native <methods>;
}

-keepclassmembers class * extends android.app.Activity {
   public void *(android.view.View);
}

-keepclassmembers enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}

-keep class * implements android.os.Parcelable {
  public static final android.os.Parcelable$Creator *;
}


# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile