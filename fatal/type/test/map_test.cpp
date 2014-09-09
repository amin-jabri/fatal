/*
 *  Copyright (c) 2014, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#include <fatal/type/map.h>

#include <fatal/test/driver.h>

namespace ftl {

template <typename> struct type1_t {};
template <typename> struct type2_t {};
template <char Value> using chr_val = std::integral_constant<char, Value>;
template <char... Args> using chr_map = build_type_map<chr_val<Args>...>;
template <int Value> using int_val = std::integral_constant<int, Value>;
template <int... Args> using int_map = build_type_map<int_val<Args>...>;
template <int Increment>
struct add {
  template <typename T> using type = int_val<T::value + Increment>;
};

#define W(value) using w##value = std::integral_constant<int, 100 + value>
W(0); W(1); W(2); W(3); W(4); W(5); W(6); W(7); W(8); W(9);
#undef W

#define X(value) using x##value = std::integral_constant<int, 100 + value>
X(0); X(1); X(2); X(3); X(4); X(5); X(6); X(7); X(8); X(9);
#undef X

#define Y(value) using y##value = std::integral_constant<int, 200 + value>
Y(0); Y(1); Y(2); Y(3); Y(4); Y(5); Y(6); Y(7); Y(8); Y(9);
#undef Y

#define Z(value) using z##value = std::integral_constant<int, 300 + value>
Z(0); Z(1); Z(2); Z(3); Z(4); Z(5); Z(6); Z(7); Z(8); Z(9);
#undef Z

template <typename T>
struct sort_transform_impl { typedef typename T::template merge_sort<> type; };
template <typename T>
using sort_transform = typename sort_transform_impl<T>::type;

//////////////
// type_map //
//////////////

typedef type_map<
  type_pair<int, bool>,
  type_pair<double, long>,
  type_pair<short, void>
> ibdlsv_map;

struct not_found_type {};

////////////////////
// build_type_map //
////////////////////

TEST(type_map, build_type_map) {
  expect_same<type_map<>, build_type_map<>>();

  expect_same<
    type_map<type_pair<int, bool>>,
    build_type_map<int, bool>
  >();

  expect_same<
    ibdlsv_map,
    build_type_map<int, bool, double, long, short, void>
  >();
}

//////////
// keys //
//////////

TEST(type_map, keys) {
  expect_same<type_map<>::keys, type_list<>>();

  expect_same<
    type_map<type_pair<int, bool>>::keys,
    type_list<int>
  >();

  expect_same<
    ibdlsv_map::keys,
    type_list<int, double, short>
  >();
}

////////////
// mapped //
////////////

TEST(type_map, mapped) {
  expect_same<type_map<>::mapped, type_list<>>();

  expect_same<
    type_map<type_pair<int, bool>>::mapped,
    type_list<bool>
  >();

  expect_same<
    ibdlsv_map::mapped,
    type_list<bool, long, void>
  >();
}

///////////////
// transform //
///////////////

TEST(type_map, transform) {
  expect_same<type_map<>, type_map<>::transform<add<10>::type>>();

  expect_same<
    type_map<>,
    type_map<>::transform<add<10>::type, add<100>::type>
  >();

  typedef build_type_map<
    int_val<0>, int_val<0>,
    int_val<1>, int_val<11>,
    int_val<3>, int_val<33>,
    int_val<5>, int_val<55>,
    int_val<7>, int_val<77>,
    int_val<9>, int_val<99>
  > map;

  expect_same<
    build_type_map<
      int_val<0>, int_val<10>,
      int_val<1>, int_val<21>,
      int_val<3>, int_val<43>,
      int_val<5>, int_val<65>,
      int_val<7>, int_val<87>,
      int_val<9>, int_val<109>
    >,
    map::transform<add<10>::type>
  >();

  expect_same<
    build_type_map<
      int_val<100>, int_val<10>,
      int_val<101>, int_val<21>,
      int_val<103>, int_val<43>,
      int_val<105>, int_val<65>,
      int_val<107>, int_val<87>,
      int_val<109>, int_val<109>
    >,
    map::transform<add<10>::type, add<100>::type>
  >();
}

//////////////////
// transform_at //
//////////////////

TEST(type_map, transform_at) {
  expect_same<
    type_map<>,
    type_map<>::transform_at<int_val<0>, add<10>::type>
  >();

  expect_same<
    type_map<>,
    type_map<>::transform_at<int_val<0>, add<10>::type, add<100>::type>
  >();

  typedef build_type_map<
    int_val<0>, int_val<0>,
    int_val<1>, int_val<11>,
    int_val<3>, int_val<33>,
    int_val<5>, int_val<55>,
    int_val<7>, int_val<77>,
    int_val<9>, int_val<99>
  > map;

  expect_same<
    map,
    map::transform_at<int_val<1000>, add<10>::type>
  >();

  expect_same<
    map,
    map::transform_at<int_val<1000>, add<10>::type, add<100>::type>
  >();

  expect_same<
    build_type_map<
      int_val<0>, int_val<10>,
      int_val<1>, int_val<11>,
      int_val<3>, int_val<33>,
      int_val<5>, int_val<55>,
      int_val<7>, int_val<77>,
      int_val<9>, int_val<99>
    >,
    map::transform_at<int_val<0>, add<10>::type>
  >();

  expect_same<
    build_type_map<
      int_val<100>, int_val<10>,
      int_val<1>, int_val<11>,
      int_val<3>, int_val<33>,
      int_val<5>, int_val<55>,
      int_val<7>, int_val<77>,
      int_val<9>, int_val<99>
    >,
    map::transform_at<int_val<0>, add<10>::type, add<100>::type>
  >();

  expect_same<
    build_type_map<
      int_val<0>, int_val<0>,
      int_val<1>, int_val<11>,
      int_val<3>, int_val<33>,
      int_val<5>, int_val<55>,
      int_val<7>, int_val<77>,
      int_val<9>, int_val<109>
    >,
    map::transform_at<int_val<9>, add<10>::type>
  >();

  expect_same<
    build_type_map<
      int_val<0>, int_val<0>,
      int_val<1>, int_val<11>,
      int_val<3>, int_val<33>,
      int_val<105>, int_val<65>,
      int_val<7>, int_val<77>,
      int_val<9>, int_val<99>
    >,
    map::transform_at<int_val<5>, add<10>::type, add<100>::type>
  >();
}

////////////
// invert //
////////////

TEST(type_map, invert) {
  typedef type_map<
    type_pair<int, bool>,
    type_pair<float, long>
  > map;

  typedef type_map<
    type_pair<bool, int>,
    type_pair<long, float>
  > expected1;
  expect_same<expected1, map::invert<>>();

  typedef type_map<
    type_pair<type2_t<bool>, type1_t<int>>,
    type_pair<type2_t<long>, type1_t<float>>
  > expected2;
  expect_same<expected2, map::invert<type1_t, type2_t>>();
}

//////////
// find //
//////////

TEST(type_map, find) {
  expect_same<type_map<>::find<int, not_found_type>, not_found_type>();
  expect_same<ibdlsv_map::find<int, not_found_type>, bool>();
  expect_same<ibdlsv_map::find<double, not_found_type>, long>();
  expect_same<ibdlsv_map::find<short, not_found_type>, void>();
  expect_same<ibdlsv_map::find<bool, not_found_type>, not_found_type>();
}

//////////////
// contains //
//////////////

TEST(type_map, contains) {
  EXPECT_FALSE((type_map<>::contains<int>::value));
  EXPECT_TRUE((ibdlsv_map::contains<int>::value));
  EXPECT_TRUE((ibdlsv_map::contains<double>::value));
  EXPECT_TRUE((ibdlsv_map::contains<short>::value));
  EXPECT_FALSE((ibdlsv_map::contains<bool>::value));
}

////////////////
// push_front //
////////////////

TEST(type_map, push_front) {
  typedef type_map<
    type_pair<int, bool>,
    type_pair<float, double>
  > map;

  typedef type_map<
    type_pair<short, long>,
    type_pair<int, bool>,
    type_pair<float, double>
  > expected1;
  expect_same<expected1, map::push_front<short, long>>();

  typedef type_map<
    type_pair<short, long>,
    type_pair<int, bool>,
    type_pair<float, double>
  > expected2;
  expect_same<expected2, map::push_front<type_pair<short, long>>>();
}

///////////////
// push_back //
///////////////

TEST(type_map, push_back) {
  typedef type_map<
    type_pair<int, bool>,
    type_pair<float, double>
  > map;

  typedef type_map<
    type_pair<int, bool>,
    type_pair<float, double>,
    type_pair<short, long>
  > expected1;
  expect_same<expected1, map::push_back<short, long>>();

  typedef type_map<
    type_pair<int, bool>,
    type_pair<float, double>,
    type_pair<short, long>
  > expected2;
  expect_same<expected2, map::push_back<type_pair<short, long>>>();
}

////////////
// insert //
////////////

TEST(type_map, insert) {
  expect_same<
    build_type_map<int, bool>,
    type_map<>
      ::insert<int, bool>
  >();
  expect_same<
    build_type_map<int, bool, double, long>,
    type_map<>
      ::insert<int, bool>
      ::insert<double, long>
  >();
  expect_same<
    build_type_map<int, bool, double, long, short, void>,
    type_map<>
      ::insert<int, bool>
      ::insert<double, long>
      ::insert<short, void>
  >();

  expect_same<
    build_type_map<int, bool>,
    type_map<>
      ::insert<type_pair<int, bool>>
  >();
  expect_same<
    build_type_map<int, bool, double, long>,
    type_map<>
      ::insert<type_pair<int, bool>>
      ::insert<type_pair<double, long>>
  >();
  expect_same<
    build_type_map<int, bool, double, long, short, void>,
    type_map<>
      ::insert<type_pair<int, bool>>
      ::insert<type_pair<double, long>>
      ::insert<type_pair<short, void>>
  >();
}

///////////////////
// insert_sorted //
///////////////////

TEST(type_map, insert_sorted) {
  expect_same<
    build_type_map<int_val<1>, void>,
    type_map<>::insert_sorted<int_val<1>, void>
  >();

  expect_same<
    build_type_map<
      int_val<1>, bool,
      int_val<2>, long,
      int_val<3>, double
    >,
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double
    >::insert_sorted<int_val<2>, long>
  >();

  expect_same<
    build_type_map<
      int_val<0>, int,
      int_val<1>, bool,
      int_val<3>, double
    >,
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double
    >::insert_sorted<int_val<0>, int>
  >();

  expect_same<
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double,
      int_val<4>, short
    >,
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double
    >::insert_sorted<int_val<4>, short>
  >();

  expect_same<
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double,
      int_val<3>, void
    >,
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double
    >::insert_sorted<int_val<3>, void>
  >();

  expect_same<
    build_type_map<
      int_val<3>, double,
      int_val<2>, long,
      int_val<1>, bool
    >,
    build_type_map<
      int_val<3>, double,
      int_val<1>, bool
    >::insert_sorted<int_val<2>, long, constants_comparison_gt>
  >();

  expect_same<
    build_type_map<
      int_val<3>, double,
      int_val<1>, bool,
      int_val<0>, int
    >,
    build_type_map<
      int_val<3>, double,
      int_val<1>, bool
    >::insert_sorted<int_val<0>, int, constants_comparison_gt>
  >();

  expect_same<
    build_type_map<
      int_val<4>, short,
      int_val<3>, double,
      int_val<1>, bool
    >,
    build_type_map<
      int_val<3>, double,
      int_val<1>, bool
    >::insert_sorted<int_val<4>, short, constants_comparison_gt>
  >();

  expect_same<
    build_type_map<
      int_val<3>, double,
      int_val<3>, void,
      int_val<1>, bool
    >,
    build_type_map<
      int_val<3>, double,
      int_val<1>, bool
    >::insert_sorted<int_val<3>, void, constants_comparison_gt>
  >();
}

////////////////////////
// insert_pair_sorted //
////////////////////////

TEST(type_map, insert_pair_sorted) {
  expect_same<
    build_type_map<int_val<1>, void>,
    type_map<>::insert_pair_sorted<type_pair<int_val<1>, void>>
  >();

  expect_same<
    build_type_map<
      int_val<1>, bool,
      int_val<2>, long,
      int_val<3>, double
    >,
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double
    >::insert_pair_sorted<type_pair<int_val<2>, long>>
  >();

  expect_same<
    build_type_map<
      int_val<0>, int,
      int_val<1>, bool,
      int_val<3>, double
    >,
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double
    >::insert_pair_sorted<type_pair<int_val<0>, int>>
  >();

  expect_same<
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double,
      int_val<4>, short
    >,
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double
    >::insert_pair_sorted<type_pair<int_val<4>, short>>
  >();

  expect_same<
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double,
      int_val<3>, void
    >,
    build_type_map<
      int_val<1>, bool,
      int_val<3>, double
    >::insert_pair_sorted<type_pair<int_val<3>, void>>
  >();

  expect_same<
    build_type_map<
      int_val<3>, double,
      int_val<2>, long,
      int_val<1>, bool
    >,
    build_type_map<
      int_val<3>, double,
      int_val<1>, bool
    >::insert_pair_sorted<
      type_pair<int_val<2>, long>,
      constants_comparison_gt
    >
  >();

  expect_same<
    build_type_map<
      int_val<3>, double,
      int_val<1>, bool,
      int_val<0>, int
    >,
    build_type_map<
      int_val<3>, double,
      int_val<1>, bool
    >::insert_pair_sorted<
      type_pair<int_val<0>, int>,
      constants_comparison_gt
    >
  >();

  expect_same<
    build_type_map<
      int_val<4>, short,
      int_val<3>, double,
      int_val<1>, bool
    >,
    build_type_map<
      int_val<3>, double,
      int_val<1>, bool
    >::insert_pair_sorted<
      type_pair<int_val<4>, short>,
      constants_comparison_gt
    >
  >();

  expect_same<
    build_type_map<
      int_val<3>, double,
      int_val<3>, void,
      int_val<1>, bool
    >,
    build_type_map<
      int_val<3>, double,
      int_val<1>, bool
    >::insert_pair_sorted<
      type_pair<int_val<3>, void>,
      constants_comparison_gt
    >
  >();
}

/////////////
// replace //
/////////////

TEST(type_map, replace) {
  expect_same<
    build_type_map<>,
    build_type_map<>::replace<int, double>
  >();

  expect_same<
    build_type_map<int, double>,
    build_type_map<int, double>
      ::replace<bool, short>
  >();

  expect_same<
    build_type_map<int, double, long, float>,
    build_type_map<int, double, long, float>
      ::replace<bool, short>
  >();

  expect_same<
    build_type_map<int, double>,
    build_type_map<int, long>
      ::replace<int, double>
  >();

  expect_same<
    build_type_map<int, double, float, short, int, double>,
    build_type_map<int, long, float, short, int, bool>
      ::replace<int, double>
  >();
}

////////////
// remove //
////////////

TEST(type_map, remove) {
  expect_same<build_type_map<>, build_type_map<>::remove<>>();
  expect_same<build_type_map<>, build_type_map<>::remove<int>>();
  expect_same<build_type_map<>, build_type_map<>::remove<int, short>>();

  expect_same<
    build_type_map<int, bool>,
    build_type_map<int, bool, short, long>
      ::remove<short>
  >();
  expect_same<
    build_type_map<int, bool, short, long>,
    build_type_map<int, bool, short, long>
      ::remove<>
  >();
  expect_same<
    build_type_map<int, bool, short, long>,
    build_type_map<int, bool, short, long>
      ::remove<bool>
  >();
  expect_same<
    build_type_map<int, bool, short, long>,
    build_type_map<int, bool, short, long>
      ::remove<long>
  >();
  expect_same<
    build_type_map<int, bool, short, long>,
    build_type_map<int, bool, short, long>
      ::remove<void>
  >();
  expect_same<
    build_type_map<int, bool, short, long>,
    build_type_map<int, bool, short, long>
      ::remove<bool, long>
  >();
  expect_same<
    build_type_map<int, bool, short, long>,
    build_type_map<int, bool, short, long>
      ::remove<void, double>
  >();

  expect_same<
    build_type_map<>,
    build_type_map<int, bool, short, long>
      ::remove<int, short>
  >();
  expect_same<
    build_type_map<>,
    build_type_map<int, bool, short, long>
      ::remove<void, double, int, short>
  >();
}

////////////////
// merge_sort //
////////////////

TEST(type_map, merge_sort) {
  typedef type_map<
    type_pair<int_val<0>, void>,
    type_pair<int_val<1>, short>,
    type_pair<int_val<4>, double>,
    type_pair<int_val<2>, bool>,
    type_pair<int_val<1>, int>,
    type_pair<int_val<3>, float>
  > map;

  typedef type_map<
    type_pair<int_val<0>, void>,
    type_pair<int_val<1>, short>,
    type_pair<int_val<1>, int>,
    type_pair<int_val<2>, bool>,
    type_pair<int_val<3>, float>,
    type_pair<int_val<4>, double>
  > expected;

  expect_same<
    expected,
    map::merge_sort<>
  >();

  typedef type_map<
    type_pair<int_val<4>, double>,
    type_pair<int_val<3>, float>,
    type_pair<int_val<2>, bool>,
    type_pair<int_val<1>, short>,
    type_pair<int_val<1>, int>,
    type_pair<int_val<0>, void>
  > expected_reverse;

  expect_same<
    expected_reverse,
    map::merge_sort<constants_comparison_gt>
  >();
}

/////////////
// cluster //
/////////////

TEST(type_map, cluster) {
  expect_same<
    build_type_map<>::merge_sort<>,
    build_type_map<>::cluster<>::merge_sort<>
  >();

  expect_same<
    build_type_map<>,
    build_type_map<>::cluster<>::merge_sort<>::transform<sort_transform>
  >();

  expect_same<
    build_type_map<
      x0, type_list<y0>
    >::merge_sort<>,
    build_type_map<
      x0, y0
    >::cluster<>::merge_sort<>::transform<sort_transform>
  >();

  expect_same<
    build_type_map<
      x0, type_list<y0>, x1, type_list<y1>,
      x2, type_list<y2>, x3, type_list<y3>,
      x4, type_list<y4>, x5, type_list<y5>,
      x6, type_list<y6>, x7, type_list<y7>,
      x8, type_list<y8>, x9, type_list<y9>
    >::merge_sort<>,
    build_type_map<
      x0, y0, x1, y1, x2, y2, x3, y3, x4, y4,
      x5, y5, x6, y6, x7, y7, x8, y8, x9, y9
    >::cluster<>::merge_sort<>::transform<sort_transform>
  >();

  expect_same<
    build_type_map<
      x1, type_list<y0, y1, y2, y3, y4>::merge_sort<>,
      x2, type_list<x8, x9, z0, z2>::merge_sort<>,
      x3, type_list<y5, y6, y7, y8, y9>::merge_sort<>,
      x4, type_list<z1, z2, z3, z4, z5, z6, z7, z8, z9>::merge_sort<>,
      x5, type_list<z9, y0, x1>::merge_sort<>
    >::merge_sort<>,
    build_type_map<
      x1, y0, x1, y1, x1, y2, x1, y3, x1, y4,
      x2, x8, x2, x9, x2, z0, x2, z2,
      x3, y5, x3, y6, x3, y7, x3, y8, x3, y9,
      x4, z1, x4, z2, x4, z3, x4, z4, x4, z5, x4, z6, x4, z7, x4, z8, x4, z9,
      x5, z9, x5, y0, x5, x1
    >::cluster<>::merge_sort<>::transform<sort_transform>
  >();
}

///////////
// visit //
///////////

template <typename TExpectedKey, typename TExpectedMapped>
struct visit_test_visitor {
  template <typename TActualKey, typename TActualMapped>
  void operator ()(type_pair<TActualKey, TActualMapped>) const {
    EXPECT_SAME(TExpectedKey, TActualKey);
    EXPECT_SAME(TExpectedMapped, TActualMapped);
  }
};


template <typename TMap, typename TKey, typename TMapped = void>
void check_visit(bool expected = true) {
  auto const actual = TMap::template visit<TKey>(
    visit_test_visitor<TKey, TMapped>()
  );

  EXPECT_EQ(expected, actual);
}

TEST(type_map, visit) {
  typedef build_type_map<
    int, bool,
    double, float,
    std::string, std::vector<int>
  > map0;

  check_visit<map0, int, bool>();
  check_visit<map0, double, float>();
  check_visit<map0, std::string, std::vector<int>>();

  check_visit<map0, long>(false);
  check_visit<map0, int &>(false);
  check_visit<map0, double const &>(false);
  check_visit<map0, std::string &&>(false);
}

//////////////////////////
// binary_search::exact //
//////////////////////////

template <typename T>
struct bs_visitor {
  template <T Key, T Mapped, std::size_t Index>
  void operator ()(
    indexed_type_tag<
      type_pair<
        std::integral_constant<T, Key>,
        std::integral_constant<T, Mapped>
      >,
      Index
    >,
    T const &needle,
    T &key,
    T &mapped,
    std::size_t &index
  ) {
    key = Key;
    mapped = Mapped;
    index = Index;
  };
};

template <
  typename T,
  bool Result, T Needle, T ExpectedMapped, std::size_t ExpectedIndex,
  typename TMap, T Empty
>
void check_bs_exact() {
  auto key = Empty;
  auto mapped = Empty;
  std::size_t index = TMap::size;

  auto result = TMap::template binary_search<type_value_comparer>::exact(
    Needle, bs_visitor<T>(), key, mapped, index
  );

  auto const expectedResult = Result;
  EXPECT_EQ(expectedResult, result);
  auto const expectedKey = Result ? Needle : Empty;
  EXPECT_EQ(expectedKey, key);
  auto const expectedMapped = Result ? ExpectedMapped : Empty;
  EXPECT_EQ(expectedMapped, mapped);
  auto const expectedIndex = ExpectedIndex;
  EXPECT_EQ(expectedIndex, index);
}

TEST(type_map, binary_search_exact) {
  typedef chr_map<> empty;

  LOG(INFO) << "empty";
  check_bs_exact<char, false, '-', '\0', empty::size, empty, '\0'>();
  check_bs_exact<int, false, 3, -1, empty::size, empty, -1>();

  typedef chr_map<'x', 'X'> one;

  LOG(INFO) << "one";
  check_bs_exact<char, false, '-', '\0', one::size, one, '\0'>();
  check_bs_exact<char, true, 'x', 'X', 0, one, '\0'>();

  typedef chr_map<'x', 'X', 'y', 'Y'> two;

  LOG(INFO) << "two";
  check_bs_exact<char, false, '-', '\0', two::size, two, '\0'>();
  check_bs_exact<char, true, 'x', 'X', 0, two, '\0'>();
  check_bs_exact<char, true, 'y', 'Y', 1, two, '\0'>();

  typedef chr_map<'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'> aeiou;

  LOG(INFO) << "aeiou";
  check_bs_exact<char, false, 'x', '\0', aeiou::size, aeiou, '\0'>();

  check_bs_exact<char, true, 'a', 'A', 0, aeiou, '\0'>();
  check_bs_exact<char, true, 'e', 'E', 1, aeiou, '\0'>();
  check_bs_exact<char, true, 'i', 'I', 2, aeiou, '\0'>();
  check_bs_exact<char, true, 'o', 'O', 3, aeiou, '\0'>();
  check_bs_exact<char, true, 'u', 'U', 4, aeiou, '\0'>();

  typedef int_map<
    3, 2, 7, 3, 31, 5, 127, 7, 8191, 13, 131071, 17, 524287, 19, 2147483647, 31
  > mp;

  LOG(INFO) << "mp";
  check_bs_exact<int, false, -1, -1, mp::size, mp, -1>();
  check_bs_exact<int, false, 0, -1, mp::size, mp, -1>();
  check_bs_exact<int, false, 63, -1, mp::size, mp, -1>();
  check_bs_exact<int, false, 63, -1, mp::size, mp, -1>();

  check_bs_exact<int, true, 3, 2, 0, mp, -1>();
  check_bs_exact<int, true, 7, 3, 1, mp, -1>();
  check_bs_exact<int, true, 31, 5, 2, mp, -1>();
  check_bs_exact<int, true, 127, 7, 3, mp, -1>();
  check_bs_exact<int, true, 8191, 13, 4, mp, -1>();
  check_bs_exact<int, true, 131071, 17, 5, mp, -1>();
  check_bs_exact<int, true, 524287, 19, 6, mp, -1>();
  check_bs_exact<int, true, 2147483647, 31, 7, mp, -1>();
}

////////////////////////////////
// binary_search::lower_bound //
////////////////////////////////

template <
  typename T,
  bool Result, T Needle, T ExpectedKey, T ExpectedMapped,
  std::size_t ExpectedIndex, typename TMap, T Empty
>
void check_bs_lower_bound() {
  auto key = Empty;
  auto mapped = Empty;
  std::size_t index = TMap::size;

  auto result = TMap::template binary_search<type_value_comparer>::lower_bound(
    Needle, bs_visitor<T>(), key, mapped, index
  );

  auto const expectedResult = Result;
  EXPECT_EQ(expectedResult, result);
  auto const expectedKey = ExpectedKey;
  EXPECT_EQ(expectedKey, key);
  auto const expectedMapped = ExpectedMapped;
  EXPECT_EQ(expectedMapped, mapped);
  auto const expectedIndex = ExpectedIndex;
  EXPECT_EQ(expectedIndex, index);
}

TEST(type_map, binary_search_lower_bound) {
  typedef chr_map<> empty;

  LOG(INFO) << "empty";
  check_bs_lower_bound<char, false, '-', '\0', '\0', empty::size,
    empty, '\0'
  >();
  check_bs_lower_bound<int,  false, 3,   -1,   -1, empty::size, empty, -1>();

  typedef chr_map<'x', 'X'> one;

  LOG(INFO) << "one";
  check_bs_lower_bound<char, false, 'w', '\0', '\0', one::size, one, '\0'>();
  check_bs_lower_bound<char, true,  'x', 'x',  'X',  0, one, '\0'>();
  check_bs_lower_bound<char, true,  'y', 'x',  'X',  0, one, '\0'>();

  typedef chr_map<'x', 'X', 'y', 'Y'> two;

  LOG(INFO) << "two";
  check_bs_lower_bound<char, false, 'w', '\0', '\0', two::size, two, '\0'>();
  check_bs_lower_bound<char, true,  'x', 'x',  'X',  0, two, '\0'>();
  check_bs_lower_bound<char, true,  'y', 'y',  'Y',  1, two, '\0'>();
  check_bs_lower_bound<char, true,  'z', 'y',  'Y',  1, two, '\0'>();

  typedef chr_map<'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'> aeiou;

  LOG(INFO) << "aeiou";
  check_bs_lower_bound<char, false, 'a' - 1, '\0', '\0', aeiou::size,
    aeiou, '\0'
  >();
  check_bs_lower_bound<char, true,  'a',     'a',  'A',  0, aeiou, '\0'>();
  check_bs_lower_bound<char, true,  'e',     'e',  'E',  1, aeiou, '\0'>();
  check_bs_lower_bound<char, true,  'i',     'i',  'I',  2, aeiou, '\0'>();
  check_bs_lower_bound<char, true,  'o',     'o',  'O',  3, aeiou, '\0'>();
  check_bs_lower_bound<char, true,  'u',     'u',  'U',  4, aeiou, '\0'>();
  check_bs_lower_bound<char, true,  'x',     'u',  'U',  4, aeiou, '\0'>();

  typedef int_map<
    3, 2, 7, 3, 31, 5, 127, 7, 8191, 13, 131071, 17, 524287, 19
  > mp;

  LOG(INFO) << "mp";
  check_bs_lower_bound<int, false, -1,        -1,     -1, mp::size, mp, -1>();
  check_bs_lower_bound<int, false, 0,         -1,     -1, mp::size, mp, -1>();
  check_bs_lower_bound<int, false, 2,         -1,     -1, mp::size, mp, -1>();
  check_bs_lower_bound<int, true,  3,         3,      2,  0, mp, -1>();
  check_bs_lower_bound<int, true,  4,         3,      2,  0, mp, -1>();
  check_bs_lower_bound<int, true,  6,         3,      2,  0, mp, -1>();
  check_bs_lower_bound<int, true,  7,         7,      3,  1, mp, -1>();
  check_bs_lower_bound<int, true,  8,         7,      3,  1, mp, -1>();
  check_bs_lower_bound<int, true,  30,        7,      3,  1, mp, -1>();
  check_bs_lower_bound<int, true,  31,        31,     5,  2, mp, -1>();
  check_bs_lower_bound<int, true,  32,        31,     5,  2, mp, -1>();
  check_bs_lower_bound<int, true,  126,       31,     5,  2, mp, -1>();
  check_bs_lower_bound<int, true,  127,       127,    7,  3, mp, -1>();
  check_bs_lower_bound<int, true,  128,       127,    7,  3, mp, -1>();
  check_bs_lower_bound<int, true,  8190,      127,    7,  3, mp, -1>();
  check_bs_lower_bound<int, true,  8191,      8191,   13, 4, mp, -1>();
  check_bs_lower_bound<int, true,  8192,      8191,   13, 4, mp, -1>();
  check_bs_lower_bound<int, true,  131070,    8191,   13, 4, mp, -1>();
  check_bs_lower_bound<int, true,  131071,    131071, 17, 5, mp, -1>();
  check_bs_lower_bound<int, true,  131072,    131071, 17, 5, mp, -1>();
  check_bs_lower_bound<int, true,  524286,    131071, 17, 5, mp, -1>();
  check_bs_lower_bound<int, true,  524287,    524287, 19, 6, mp, -1>();
  check_bs_lower_bound<int, true,  524288,    524287, 19, 6, mp, -1>();
}

////////////////////////////////
// binary_search::upper_bound //
////////////////////////////////

template <
  typename T,
  bool Result, T Needle, T ExpectedKey, T ExpectedMapped,
  std::size_t ExpectedIndex, typename TMap, T Empty
>
void check_bs_upper_bound() {
  auto key = Empty;
  auto mapped = Empty;
  std::size_t index = TMap::size;

  auto result = TMap::template binary_search<type_value_comparer>::upper_bound(
    Needle, bs_visitor<T>(), key, mapped, index
  );

  auto const expectedResult = Result;
  EXPECT_EQ(expectedResult, result);
  auto const expectedKey = ExpectedKey;
  EXPECT_EQ(expectedKey, key);
  auto const expectedMapped = ExpectedMapped;
  EXPECT_EQ(expectedMapped, mapped);
  auto const expectedIndex = ExpectedIndex;
  EXPECT_EQ(expectedIndex, index);
}

TEST(type_map, binary_search_upper_bound) {
  typedef chr_map<> empty;

  LOG(INFO) << "empty";
  check_bs_upper_bound<char, false, '-', '\0', '\0', empty::size,
    empty, '\0'
  >();
  check_bs_upper_bound<int,  false, 3,   -1,   -1,   empty::size, empty, -1>();

  typedef chr_map<'x', 'X'> one;

  LOG(INFO) << "one";
  check_bs_upper_bound<char, true,  'w', 'x',  'X',  0, one, '\0'>();
  check_bs_upper_bound<char, false, 'x', '\0', '\0', one::size, one, '\0'>();
  check_bs_upper_bound<char, false, 'y', '\0', '\0', one::size, one, '\0'>();

  typedef chr_map<'x', 'X', 'y', 'Y'> two;

  LOG(INFO) << "two";
  check_bs_upper_bound<char, true,  'w', 'x',  'X',  0, two, '\0'>();
  check_bs_upper_bound<char, true,  'x', 'y',  'Y',  1, two, '\0'>();
  check_bs_upper_bound<char, false, 'y', '\0', '\0', two::size, two, '\0'>();
  check_bs_upper_bound<char, false, 'z', '\0', '\0', two::size, two, '\0'>();

  typedef chr_map<'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'> aeiou;

  LOG(INFO) << "aeiou";
  check_bs_upper_bound<char, true, 'a' - 1, 'a',  'A',  0, aeiou, '\0'>();
  check_bs_upper_bound<char, true, 'a',     'e',  'E',  1, aeiou, '\0'>();
  check_bs_upper_bound<char, true, 'e',     'i',  'I',  2, aeiou, '\0'>();
  check_bs_upper_bound<char, true, 'i',     'o',  'O',  3, aeiou, '\0'>();
  check_bs_upper_bound<char, true, 'o',     'u',  'U',  4, aeiou, '\0'>();
  check_bs_upper_bound<char, false, 'u',    '\0', '\0', aeiou::size,
    aeiou, '\0'
  >();

  typedef int_map<
    3, 2, 7, 3, 31, 5, 127, 7, 8191, 13, 131071, 17, 524287, 19
  > mp;

  LOG(INFO) << "mp";
  check_bs_upper_bound<int, true,  -1,         3,      2,  0, mp, -1>();
  check_bs_upper_bound<int, true,  0,          3,      2,  0, mp, -1>();
  check_bs_upper_bound<int, true,  2,          3,      2,  0, mp, -1>();
  check_bs_upper_bound<int, true,  3,          7,      3,  1, mp, -1>();
  check_bs_upper_bound<int, true,  4,          7,      3,  1, mp, -1>();
  check_bs_upper_bound<int, true,  6,          7,      3,  1, mp, -1>();
  check_bs_upper_bound<int, true,  7,          31,     5,  2, mp, -1>();
  check_bs_upper_bound<int, true,  8,          31,     5,  2, mp, -1>();
  check_bs_upper_bound<int, true,  30,         31,     5,  2, mp, -1>();
  check_bs_upper_bound<int, true,  31,         127,    7,  3, mp, -1>();
  check_bs_upper_bound<int, true,  32,         127,    7,  3, mp, -1>();
  check_bs_upper_bound<int, true,  126,        127,    7,  3, mp, -1>();
  check_bs_upper_bound<int, true,  127,        8191,   13, 4, mp, -1>();
  check_bs_upper_bound<int, true,  128,        8191,   13, 4, mp, -1>();
  check_bs_upper_bound<int, true,  8190,       8191,   13, 4, mp, -1>();
  check_bs_upper_bound<int, true,  8191,       131071, 17, 5, mp, -1>();
  check_bs_upper_bound<int, true,  8192,       131071, 17, 5, mp, -1>();
  check_bs_upper_bound<int, true,  131070,     131071, 17, 5, mp, -1>();
  check_bs_upper_bound<int, true,  131071,     524287, 19, 6, mp, -1>();
  check_bs_upper_bound<int, true,  131072,     524287, 19, 6, mp, -1>();
  check_bs_upper_bound<int, true,  524286,     524287, 19, 6, mp, -1>();
  check_bs_upper_bound<int, false, 524287,     -1,     -1, mp::size, mp, -1>();
  check_bs_upper_bound<int, false, 524288,     -1,     -1, mp::size, mp, -1>();
}

//////////////
// type_get //
//////////////

template <typename TMap>
struct check_type_get_visitor {
  template <typename T, std::size_t Index>
  void operator ()(indexed_type_tag<T, Index> const &) {
    expect_same<T, typename type_get<Index>::template type<TMap>>();

    expect_same<
      typename TMap::contents::template at<Index>,
      typename type_get<Index>::template type<TMap>
    >();
  }
};

template <typename... Args>
void check_type_get() {
  typedef build_type_map<Args...> map;
  map::contents::foreach(check_type_get_visitor<map>());
}

TEST(type_get, type_map) {
  check_type_get<>();
  check_type_get<void, void>();
  check_type_get<int, double>();
  check_type_get<int, bool, float, long>();
  check_type_get<
    type_list<bool, int>,
    type_list<std::string, std::string>,
    type_list<std::string, std::string, void>,
    build_type_map<float, double>
  >();
}

/////////////////////////
// type_map_from::list //
/////////////////////////

TEST(type_map, type_map_from_list) {
  typedef type_list<int, bool, double> list;

  typedef type_map<
     type_pair<type1_t<int>, int>,
     type_pair<type1_t<bool>, bool>,
     type_pair<type1_t<double>, double>
  > expected1;
  expect_same<expected1, type_map_from<type1_t>::list<list>>();

  typedef type_map<
     type_pair<type1_t<int>, type2_t<int>>,
     type_pair<type1_t<bool>, type2_t<bool>>,
     type_pair<type1_t<double>, type2_t<double>>
  > expected2;
  expect_same<expected2, type_map_from<type1_t, type2_t>::list<list>>();

  typedef type_map<
     type_pair<int, int>,
     type_pair<bool, bool>,
     type_pair<double, double>
  > expected3;
  expect_same<expected3, type_map_from<>::list<list>>();
}

/////////////////////
// clustered_index //
/////////////////////

template <typename L1, typename L2, typename L3, typename L4>
struct clustered_index_metadata {
  typedef L1 l1;
  typedef L2 l2;
  typedef L3 l3;
  typedef L4 l4;
};

template <typename T> struct get_l1_impl { typedef typename T::l1 type; };
template <typename T> using get_l1 = typename get_l1_impl<T>::type;
template <typename T> struct get_l2_impl { typedef typename T::l2 type; };
template <typename T> using get_l2 = typename get_l2_impl<T>::type;
template <typename T> struct get_l3_impl { typedef typename T::l3 type; };
template <typename T> using get_l3 = typename get_l3_impl<T>::type;
template <typename T> struct get_l4_impl { typedef typename T::l4 type; };
template <typename T> using get_l4 = typename get_l4_impl<T>::type;

template <typename TInput, typename TExpected>
void check_clustered_index() {
  using expected = transform::full_recursive_type_merge_sort<TExpected>;
  using actual = transform::full_recursive_type_merge_sort<
    clustered_index<TInput, get_l1, get_l2, get_l3, get_l4>
  >;

  expect_same<expected, actual>();
}

TEST(type_map, clustered_index) {
  check_clustered_index<type_list<>, type_map<>>();

  typedef clustered_index_metadata<w0, x0, y0, z0> d0;
  typedef clustered_index_metadata<w1, x1, y1, z1> d1;
  typedef clustered_index_metadata<w2, x2, y2, z2> d2;
  typedef clustered_index_metadata<w3, x3, y3, z3> d3;
  typedef clustered_index_metadata<w4, x4, y4, z4> d4;
  typedef clustered_index_metadata<w5, x5, y5, z5> d5;
  typedef clustered_index_metadata<w6, x6, y6, z6> d6;
  typedef clustered_index_metadata<w7, x7, y7, z7> d7;
  typedef clustered_index_metadata<w8, x8, y8, z8> d8;
  typedef clustered_index_metadata<w9, x9, y9, z9> d9;

  check_clustered_index<
    type_list<d0>,
    build_type_map<
      w0, build_type_map<x0, build_type_map<y0, build_type_map<z0, d0>>>
    >
  >();

  check_clustered_index<
    type_list<d0, d1, d2, d3, d4, d5, d6, d7, d8, d9>,
    build_type_map<
      w0, build_type_map<x0, build_type_map<y0, build_type_map<z0, d0>>>,
      w1, build_type_map<x1, build_type_map<y1, build_type_map<z1, d1>>>,
      w2, build_type_map<x2, build_type_map<y2, build_type_map<z2, d2>>>,
      w3, build_type_map<x3, build_type_map<y3, build_type_map<z3, d3>>>,
      w4, build_type_map<x4, build_type_map<y4, build_type_map<z4, d4>>>,
      w5, build_type_map<x5, build_type_map<y5, build_type_map<z5, d5>>>,
      w6, build_type_map<x6, build_type_map<y6, build_type_map<z6, d6>>>,
      w7, build_type_map<x7, build_type_map<y7, build_type_map<z7, d7>>>,
      w8, build_type_map<x8, build_type_map<y8, build_type_map<z8, d8>>>,
      w9, build_type_map<x9, build_type_map<y9, build_type_map<z9, d9>>>
    >
  >();

  typedef clustered_index_metadata<w1, x1, y1, z1> d111;
  typedef clustered_index_metadata<w1, x1, y1, z2> d112;
  typedef clustered_index_metadata<w1, x1, y1, z3> d113;
  typedef clustered_index_metadata<w1, x1, y1, z4> d114;
  typedef clustered_index_metadata<w1, x1, y2, z1> d121;
  typedef clustered_index_metadata<w1, x1, y2, z2> d122;
  typedef clustered_index_metadata<w1, x1, y2, z3> d123;
  typedef clustered_index_metadata<w1, x1, y2, z4> d124;
  typedef clustered_index_metadata<w1, x1, y3, z1> d131;
  typedef clustered_index_metadata<w1, x1, y3, z2> d132;
  typedef clustered_index_metadata<w1, x1, y3, z3> d133;
  typedef clustered_index_metadata<w1, x1, y3, z4> d134;
  typedef clustered_index_metadata<w1, x1, y4, z1> d141;
  typedef clustered_index_metadata<w1, x1, y4, z2> d142;
  typedef clustered_index_metadata<w1, x1, y4, z3> d143;
  typedef clustered_index_metadata<w1, x1, y4, z4> d144;
  typedef clustered_index_metadata<w1, x2, y1, z1> d211;
  typedef clustered_index_metadata<w1, x2, y1, z2> d212;
  typedef clustered_index_metadata<w1, x2, y1, z3> d213;
  typedef clustered_index_metadata<w1, x2, y1, z4> d214;
  typedef clustered_index_metadata<w1, x2, y2, z1> d221;
  typedef clustered_index_metadata<w1, x2, y2, z2> d222;
  typedef clustered_index_metadata<w1, x2, y2, z3> d223;
  typedef clustered_index_metadata<w1, x2, y2, z4> d224;
  typedef clustered_index_metadata<w1, x2, y3, z1> d231;
  typedef clustered_index_metadata<w1, x2, y3, z2> d232;
  typedef clustered_index_metadata<w1, x2, y3, z3> d233;
  typedef clustered_index_metadata<w1, x2, y3, z4> d234;
  typedef clustered_index_metadata<w1, x2, y4, z1> d241;
  typedef clustered_index_metadata<w1, x2, y4, z2> d242;
  typedef clustered_index_metadata<w1, x2, y4, z3> d243;
  typedef clustered_index_metadata<w1, x2, y4, z4> d244;
  typedef clustered_index_metadata<w1, x3, y1, z1> d311;
  typedef clustered_index_metadata<w1, x3, y1, z2> d312;
  typedef clustered_index_metadata<w1, x3, y1, z3> d313;
  typedef clustered_index_metadata<w1, x3, y1, z4> d314;
  typedef clustered_index_metadata<w1, x3, y2, z1> d321;
  typedef clustered_index_metadata<w1, x3, y2, z2> d322;
  typedef clustered_index_metadata<w1, x3, y2, z3> d323;
  typedef clustered_index_metadata<w1, x3, y2, z4> d324;
  typedef clustered_index_metadata<w1, x3, y3, z1> d331;
  typedef clustered_index_metadata<w1, x3, y3, z2> d332;
  typedef clustered_index_metadata<w1, x3, y3, z3> d333;
  typedef clustered_index_metadata<w1, x3, y3, z4> d334;
  typedef clustered_index_metadata<w1, x3, y4, z1> d341;
  typedef clustered_index_metadata<w1, x3, y4, z2> d342;
  typedef clustered_index_metadata<w1, x3, y4, z3> d343;
  typedef clustered_index_metadata<w1, x3, y4, z4> d344;
  typedef clustered_index_metadata<w1, x4, y1, z1> d411;
  typedef clustered_index_metadata<w1, x4, y1, z2> d412;
  typedef clustered_index_metadata<w1, x4, y1, z3> d413;
  typedef clustered_index_metadata<w1, x4, y1, z4> d414;
  typedef clustered_index_metadata<w1, x4, y2, z1> d421;
  typedef clustered_index_metadata<w1, x4, y2, z2> d422;
  typedef clustered_index_metadata<w1, x4, y2, z3> d423;
  typedef clustered_index_metadata<w1, x4, y2, z4> d424;
  typedef clustered_index_metadata<w1, x4, y3, z1> d431;
  typedef clustered_index_metadata<w1, x4, y3, z2> d432;
  typedef clustered_index_metadata<w1, x4, y3, z3> d433;
  typedef clustered_index_metadata<w1, x4, y3, z4> d434;
  typedef clustered_index_metadata<w1, x4, y4, z1> d441;
  typedef clustered_index_metadata<w1, x4, y4, z2> d442;
  typedef clustered_index_metadata<w1, x4, y4, z3> d443;
  typedef clustered_index_metadata<w1, x4, y4, z4> d444;

  check_clustered_index<
    type_list<
      d111, d111, d112, d113, d114, d121, d122, d123, d124,
      d131, d132, d133, d134, d141, d142, d143, d144,
      d211, d212, d213, d214, d221, d222, d223, d224,
      d231, d231, d232, d233, d234, d241, d242, d243, d244,
      d311, d312, d313, d314, d321, d322, d323, d324,
      d331, d332, d333, d334, d341, d341, d342, d343, d344,
      d411, d412, d413, d414, d421, d422, d423, d424,
      d431, d432, d433, d434, d441, d441, d442, d443, d444
    >,
    build_type_map<
      w1, build_type_map<
        x1, build_type_map<
          y1, build_type_map<z1, d111, z1, d111, z2, d112, z3, d113, z4, d114>,
          y2, build_type_map<z1, d121, z2, d122, z3, d123, z4, d124>,
          y3, build_type_map<z1, d131, z2, d132, z3, d133, z4, d134>,
          y4, build_type_map<z1, d141, z2, d142, z3, d143, z4, d144>
        >,
        x2, build_type_map<
          y1, build_type_map<z1, d211, z2, d212, z3, d213, z4, d214>,
          y2, build_type_map<z1, d221, z2, d222, z3, d223, z4, d224>,
          y3, build_type_map<z1, d231, z1, d231, z2, d232, z3, d233, z4, d234>,
          y4, build_type_map<z1, d241, z2, d242, z3, d243, z4, d244>
        >,
        x3, build_type_map<
          y1, build_type_map<z1, d311, z2, d312, z3, d313, z4, d314>,
          y2, build_type_map<z1, d321, z2, d322, z3, d323, z4, d324>,
          y3, build_type_map<z1, d331, z2, d332, z3, d333, z4, d334>,
          y4, build_type_map<z1, d341, z1, d341, z2, d342, z3, d343, z4, d344>
        >,
        x4, build_type_map<
          y1, build_type_map<z1, d411, z2, d412, z3, d413, z4, d414>,
          y2, build_type_map<z1, d421, z2, d422, z3, d423, z4, d424>,
          y3, build_type_map<z1, d431, z2, d432, z3, d433, z4, d434>,
          y4, build_type_map<z1, d441, z1, d441, z2, d442, z3, d443, z4, d444>
        >
      >
    >
  >();
}

} // namespace ftl {
