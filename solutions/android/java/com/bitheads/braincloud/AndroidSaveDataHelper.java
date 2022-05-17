
package com.bitheads.braincloud;

import android.content.Context;


public class AndroidSaveDataHelper {
    private String TAG = AndroidSaveDataHelper;
    private SharedPreferences sp;

    /**
     * @param context
     * @param companyName
     * @param appName
     * @param wrapperName
     * @returns if init success or failed (if Context is null it will return false)
     */

    public boolean initialize(Context context, String companyName, String appName, String wrapperName) {
        if (context != null) {
            if (wrapperName == null) {
                wrapperName = "";
            }

            if (companyName == null) {
                companyName = "";
            }

            if (appName == null) {
                appName = "";
            }

            StringBuilder builder = new StringBuilder();
            builder.append(wrapperName);
            builder.append(".");
            builder.append(companyName);
            builder.append(".");
            builder.append("appName");
            sp = context.getSharedPreferences(builder.build(), MODE_PRIVATE);
            return true;
        } else {
            return false;
        }
    }

    /**
     * @param key
     * @return String data saves under the given key
     */
    public String readData(String key) {
        return sp.getString(key, "");
    }


    /**
     * @param key
     * @param data Saves given String data under given key
     */
    public void saveData(String key, String data) {
        sp.edit().put(key, data).apply();
    }

    /**
     * @param key Deletes the data for the given key
     */
    public void deleteData(String key) {
        sp.edit().putString(key, "").apply();
    }

}
