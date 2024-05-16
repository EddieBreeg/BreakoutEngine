#pragma once

#include <PCH.hpp>
#include "FieldList.hpp"
#include <nlohmann/json_fwd.hpp>

namespace brk {
	template <class T, class = void>
	struct BinaryLoader;

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

		template <class T, class = void>
		struct HasName : std::false_type
		{};
		template <class T>
		struct HasName<
			T,
			std::enable_if_t<
				std::is_same_v<decltype(T::Name), const BasicStringView<char>>>>
			: std::true_type
		{};
	} // namespace _internal

	template <class T>
	struct BinaryLoader<T, _internal::IfHasFieldList<T>>
	{
		static const uint8_t* Load(T& out_object, const uint8_t* data);

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

	template <class T>
	struct JsonLoader<T, _internal::IfHasFieldList<T>>
	{
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
		static bool LoadField(F& field, const nlohmann::json& json, const char* name);

		template <class F>
		static void SaveField(const F& field, nlohmann::json& out_json, const char* name);
	};
} // namespace brk