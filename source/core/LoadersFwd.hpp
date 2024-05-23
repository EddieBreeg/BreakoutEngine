#pragma once

#include <PCH.hpp>
#include "FieldList.hpp"
#include <nlohmann/json_fwd.hpp>

namespace brk {
	/**
	 * Provides a common interface to load/save objects from/to binary data
	 * \tparam T the type of objects to load/save
	 */
	template <class T, class = void>
	struct BinaryLoader;

	/**
	 * Provides a common interface to load/save objects from/to json data
	 */
	template <class T, class = void>
	struct JsonLoader;

	template <class Char>
	struct BasicStringView;

	namespace _internal {
		template <class T>
		struct IsFieldList : std::false_type
		{};

		template <auto... Members>
		struct IsFieldList<meta::FieldList<Members...>>
		{};
		template <auto... Members>
		struct IsFieldList<const meta::FieldList<Members...>> : std::true_type
		{};

		template <class T, class = void>
		struct IsNativeJsonCompatible : std::false_type
		{};
		template <class T>
		struct IsNativeJsonCompatible<
			T,
			std::void_t<
				std::enable_if_t<std::is_assignable_v<nlohmann::json, T>>,
				decltype(std::declval<const nlohmann::json>().get_to(
					std::declval<T&>()))>> : std::true_type
		{};

		template <class T>
		using IfHasFieldList = std::enable_if_t<IsFieldList<decltype(T::Fields)>::value>;
	} // namespace _internal

	/**
	 * This specialization is available for any type which declares a static field list
	 * named Fields. See meta::FieldList for details
	 * \tparam T The type of objects to load/save
	 * \example SceneDescription
	 */
	template <class T>
	struct BinaryLoader<T, _internal::IfHasFieldList<T>>
	{
		/**
		 * Loads an object from bytes
		 * \param out_object The output object into which the data will be loaded
		 * \param data A pointer to the binary data to load
		 * \returns A pointer to past the last byte that was read
		 */
		static const uint8_t* Load(T& out_object, const uint8_t* data);

		/**
		 * Dumps the object into the output binary stream
		 * \param object The object to serialize
		 * \param data The output binary stream
		 * \returns A pointer to past the last byte that was written
		 */
		static uint8_t* Save(const T& object, uint8_t* out_data);

	private:
		template <auto... Members>
		static const uint8_t* LoadImpl(
			const meta::FieldList<Members...>&,
			T& out_object,
			const uint8_t* data);

		template <auto... Fields>
		static uint8_t* SaveImpl(
			const meta::FieldList<Fields...>&,
			const T& obj,
			uint8_t* out_data);
	};

	/**
	 * This specialization is available for any type which declares a static field list
	 * named Fields. See meta::FieldList for details
	 * \example SceneDescription
	 */
	template <class T>
	struct JsonLoader<T, _internal::IfHasFieldList<T>>
	{
		/**
		 * Loads the object from a json structure
		 * \param out_object The object to load
		 * \param data The input data in json form
		 * \result true if all fields were loaded successfully, false otherwise
		 */
		static bool Load(T& out_object, const nlohmann::json& data);

		static void Save(const T& object, nlohmann::json& out_json);

	private:
		template <auto... Fields, size_t... I>
		static bool LoadImpl(
			const meta::FieldList<Fields...>& list,
			std::index_sequence<I...>,
			T& out_object,
			const nlohmann::json& json);

		template <auto... Fields, size_t... I>
		static void SaveImpl(
			const meta::FieldList<Fields...>& list,
			std::index_sequence<I...>,
			const T& object,
			nlohmann::json& out_json);

		template <class F>
		static void SaveField(const F& field, nlohmann::json& out_json, const char* name);
	};

	template <class K, class V>
	bool Visit(K&& key, const nlohmann::json& json, V& out_value);
} // namespace brk