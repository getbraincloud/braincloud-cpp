// Copyright 2016 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <stddef.h>
#include <vector>
#include <string>

namespace BrainCloud
{
    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudGlobalApp
    {
    public:
        BrainCloudGlobalApp(BrainCloudClient* in_client);

        /**
         * Read game's global properties
         *
         * Service Name - GlobalApp
         * Service Operation - ReadProperties
         *
         * @param in_callback The method to be invoked when the server response is received
         */
        void readProperties(IServerCallback * in_callback = NULL);

        /**
         * Returns a list of properties, identified by the property names provided.
         * If a property from the list isn't found, it just isn't returned (no error).
         *
         * Service Name - GlobalApp
         * Service Operation - READ_SELECTED_PROPERTIES
         * 
         * @param propertyNames Specifies which properties to return
         * @param in_callback The method to be invoked when the server response is received
         */
        void readSelectedProperties(const std::vector<std::string> &propertyNames, IServerCallback * in_callback = NULL);

        /**
         * Returns a list of properties, identified by the categories provided.
         * If a category from the list isn't found, it just isn't returned (no error).
         *
         * Service Name - GlobalApp
         * Service Operation - READ_PROPERTIES_IN_CATEGORIES
         * 
         * @param categories Specifies which category to return
         * @param in_callback The method to be invoked when the server response is received
         */
        void readPropertiesInCategories(const std::vector<std::string> &categories, IServerCallback * in_callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
