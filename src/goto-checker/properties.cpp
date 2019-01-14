/*******************************************************************\

Module: Properties

Author: Daniel Kroening, Peter Schrammel

\*******************************************************************/

/// \file
/// Properties

#include "properties.h"

#include <util/invariant.h>
#include <util/json.h>
#include <util/xml.h>

std::string as_string(resultt result)
{
  switch(result)
  {
  case resultt::UNKNOWN:
    return "UNKNOWN";
  case resultt::PASS:
    return "PASS";
  case resultt::FAIL:
    return "FAIL";
  case resultt::ERROR:
    return "ERROR";
  }

  UNREACHABLE;
}

std::string as_string(property_statust status)
{
  switch(status)
  {
  case property_statust::NOT_CHECKED:
    return "NOT CHECKED";
  case property_statust::UNKNOWN:
    return "UNKNOWN";
  case property_statust::NOT_REACHABLE:
    return "UNREACHABLE";
  case property_statust::PASS:
    return "PASS";
  case property_statust::FAIL:
    return "FAIL";
  case property_statust::ERROR:
    return "ERROR";
  }

  UNREACHABLE;
}

property_infot::property_infot(
  goto_programt::const_targett pc,
  std::string description,
  property_statust status)
  : pc(pc), description(std::move(description)), status(status)
{
}

/// Return the properties in the goto model and initialize them to NOT_CHECKED
propertiest initialize_properties(const abstract_goto_modelt &goto_model)
{
  propertiest properties;
  const auto &goto_functions = goto_model.get_goto_functions();
  for(const auto &function_pair : goto_functions.function_map)
  {
    // don't collect properties from inlined functions
    if(function_pair.second.is_inlined())
      continue;

    const goto_programt &goto_program = function_pair.second.body;

    // need pointer to goto instruction
    forall_goto_program_instructions(i_it, goto_program)
    {
      if(!i_it->is_assert())
        continue;

      std::string description = id2string(i_it->source_location.get_comment());
      if(description.empty())
        description = "assertion";
      properties.emplace(
        i_it->source_location.get_property_id(),
        property_infot{i_it, description, property_statust::NOT_CHECKED});
    }
  }
  return properties;
}

std::string
as_string(const irep_idt &property_id, const property_infot &property_info)
{
  return "[" + id2string(property_id) + "] " + property_info.description +
         ": " + as_string(property_info.status);
}

xmlt xml(const irep_idt &property_id, const property_infot &property_info)
{
  xmlt xml_result("result");
  xml_result.set_attribute("property", id2string(property_id));
  xml_result.set_attribute("status", as_string(property_info.status));
  return xml_result;
}

json_objectt
json(const irep_idt &property_id, const property_infot &property_info)
{
  json_objectt result;
  result["property"] = json_stringt(property_id);
  result["description"] = json_stringt(property_info.description);
  result["status"] = json_stringt(as_string(property_info.status));
  return result;
}

std::size_t
count_properties(const propertiest &properties, property_statust status)
{
  std::size_t count = 0;
  for(const auto &property_pair : properties)
  {
    if(property_pair.second.status == status)
      ++count;
  }
  return count;
}