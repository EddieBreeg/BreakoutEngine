template <class T>
struct brk::BinaryLoader<T, std::enable_if_t<std::is_integral_v<T> && (sizeof(T) > 1)>>
{
	static const uint8_t* Load(T& out_value, const uint8_t* data)
	{
		out_value = 0;
		for (int i = 0; i < sizeof(T); i++)
		{
			out_value = (out_value << 8) | *data++;
		}
		return data;
	}

	static uint8_t* Save(const T value, uint8_t* out_data)
	{
		for (int i = 8 * (sizeof(T) - 1); i > -1; i -= 8)
		{
			*out_data++ = (value >> i) & 0xff;
		}
		return out_data;
	}
};

template <class T>
struct brk::BinaryLoader<T, std::enable_if_t<std::is_integral_v<T> && (sizeof(T) == 1)>>
{
	static const uint8_t* Load(T& out_value, const uint8_t* data)
	{
		out_value = static_cast<T>(*data++);
		return data;
	}

	static uint8_t* Save(const T value, uint8_t* out_data)
	{
		*out_data++ = static_cast<uint8_t>(value);
		return out_data;
	}
};

template <class E>
struct brk::BinaryLoader<E, std::enable_if_t<std::is_enum_v<E>>>
{
	static const uint8_t* Load(E& out_value, const uint8_t* data)
	{
		using TInt = std::underlying_type_t<E>;
		TInt tmp;
		data = BinaryLoader<TInt>::Load(tmp, data);
		out_value = static_cast<E>(tmp);
		return data;
	}

	static uint8_t* Save(const E value, uint8_t* out_data)
	{
		using TInt = std::underlying_type_t<E>;
		return BinaryLoader<TInt>::Save(static_cast<TInt>(value), out_data);
	}
};

template <class T, size_t N>
struct brk::BinaryLoader<T[N]>
{
	static const uint8_t* Load(T (&out_values)[N], const uint8_t* data)
	{
		for (size_t i = 0; i < N; i++)
		{
			data = BinaryLoader<T>::Load(out_values[N], data);
		}
		return data;
	}

	static uint8_t* Save(const T (&arr)[N], uint8_t* data)
	{
		for (const T& val : arr)
		{
			data = BinaryLoader<T>::Save(val, data);
		}
		return data;
	}
};

template <class T>
const uint8_t* brk::BinaryLoader<T, brk::_internal::IfHasFieldList<T>>::Load(
	T& out,
	const uint8_t* data)
{
	return LoadImpl(T::Fields, out_object, data);
}

template <class T>
uint8_t* brk::BinaryLoader<T, brk::_internal::IfHasFieldList<T>>::Save(
	const T& object,
	uint8_t* out_data)
{
	return SaveImpl(T::Fields, object, out_data);
}

template <class T>
template <auto... Members>
const uint8_t* brk::BinaryLoader<T, brk::_internal::IfHasFieldList<T>>::LoadImpl(
	const meta::FieldList<Members...>&,
	T& out_object,
	const uint8_t* data)
{
	return (
		(data = BinaryLoader<MemberType<Members>>::Load(out_object.*Members, data)),
		...);
}

template <class T>
template <auto... Fields>
inline uint8_t* brk::BinaryLoader<T, brk::_internal::IfHasFieldList<T>>::SaveImpl(
	const meta::FieldList<Fields...>&,
	const T& obj,
	uint8_t* out_data)
{
	return (
		(out_data = BinaryLoader<MemberType<Fields>>::Save(obj.*Fields, out_data)),
		...);
}

namespace brk {
	template <class T>
	T JsonLoader<T, _internal::IfHasFieldList<T>>::Load(const nlohmann::json& data)
	{
		return LoadImpl(T::Fields, std::make_index_sequence<T::Fields.Count>{}, data);
	}

	template <class T>
	nlohmann::json JsonLoader<T, _internal::IfHasFieldList<T>>::Save(const T& object)
	{
		return SaveImpl(T::Fields, std::make_index_sequence<T::Fields.Count>{}, object);
	}

	template <class T>
	template <auto... Fields, size_t... I>
	T JsonLoader<T, _internal::IfHasFieldList<T>>::LoadImpl(
		const meta::FieldList<Fields...>& list,
		std::index_sequence<I...>,
		const nlohmann::json& json)
	{
		T result;
		(LoadField(result.*Fields, json, list.m_Names[I]), ...);
		return result;
	}

	template <class T>
	template <auto... Fields, size_t... I>
	nlohmann::json JsonLoader<T, _internal::IfHasFieldList<T>>::SaveImpl(
		const meta::FieldList<Fields...>& list,
		std::index_sequence<I...>,
		const T& object)
	{
		nlohmann::json result;
		(SaveField(object.*Fields, result, list.m_Names[I]), ...);
		return result;
	}

	template <class T>
	template <class F>
	void JsonLoader<T, _internal::IfHasFieldList<T>>::LoadField(
		F& field,
		const nlohmann::json& json,
		const char* name)
	{
		const auto iter = json.find(name);
		if (iter == json.end())
			return;
		if constexpr (_internal::IsNativeJsonCompatible<F>::value)
		{
			iter->get_to(field);
		}
		else
		{
			field = std::move(JsonLoader<F>::Load(*iter));
		}
	}

	template <class T>
	template <class F>
	void JsonLoader<T, _internal::IfHasFieldList<T>>::SaveField(
		const F& field,
		nlohmann::json& out_json,
		const char* name)
	{
		if constexpr (_internal::IsNativeJsonCompatible<F>::value)
		{
			out_json[name] = field;
		}
		else
		{
			out_json[name] = JsonLoader<F>::Save(field);
		}
	}
} // namespace brk
